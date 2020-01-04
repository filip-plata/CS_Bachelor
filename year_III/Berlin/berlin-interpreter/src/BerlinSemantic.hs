module BerlinSemantic where

import Control.Monad
import Control.Monad.State.Lazy
import Control.Monad.Except
import Control.Monad.Reader
import Control.Monad.Identity
import Control.Applicative

import qualified Data.Map as M
import Data.Maybe
import Data.List

import AbsGrammar

import BerlinCore


type ProgramVal = ExpVal


runInterpreter :: Interpreter a -> Either BerlinRuntimeError a
runInterpreter c = runIdentity $ runExceptT $ runReaderT c $ M.singleton "toComplex" $ FunVal fromIntToComplex

class Interpret a where
  eval :: a -> BerlinResult

instance Interpret Program where
  eval = evalProgram

instance Interpret Exp where
  eval = evalExp


evalProgram :: Program -> BerlinResult
evalProgram (Main loads decl exp) = do
  env <- ask
  newEnv <- evalDecs env decl
  local (\env -> newEnv) (eval exp)


evalDec :: Env -> Dec -> BerlinEnv
evalDec env d =
  case d of
    DecT dt -> evalDecT env dt
    DecAlgType _ cons -> foldM evalAlgConstructor M.empty cons >>=
                         \env1 -> return $ (M.union env env1)

evalAlgConstructor :: Env -> AlgTypeConsDec -> BerlinEnv
evalAlgConstructor env (AlgTypeDec i@(AlgTypeIdent ident) types) =
  do
    algVal <- foldM (\v _ -> return $ FunVal (\x -> addToAlgClosure v x))
                    (AlgVal ident []) types
    return $ M.insert ident algVal env

addToAlgClosure :: ExpVal -> ExpVal -> BerlinResult
addToAlgClosure (AlgVal ident cl) exp = return $ AlgVal ident (exp:cl)
addToAlgClosure _ _ = throwError InvalidTypes

evalDecT :: Env -> DecTer -> BerlinEnv
evalDecT env d = do
  case d of
    DecVal (BIdent ident) _ exp ->
      local (\_ -> env) (eval exp) >>=
      \e -> return $ M.insert ident e env
    DecFun (BIdent ident ) args _ exp ->
      local (\_ -> env) (eval (desugarToLambda args exp)) >>=
      \e -> return $ M.insert ident e env

evalDecs :: Env -> [Dec] -> BerlinEnv
evalDecs = foldM evalDec

evalDecTs :: Env -> [DecTer] -> BerlinEnv
evalDecTs = foldM evalDecT


evalNum :: Numeral -> BerlinResult
evalNum (NumInt int) = return $ IntVal int
evalNum (NumComplex (IntComp c)) = return $ ComplexIntVal $ Complex 0 c

evalConstant :: Constant -> BerlinResult
evalConstant (ConNum num) = evalNum num
evalConstant (ConChar c) = return $ CharVal c
evalConstant (ConBool b) = BoolVal <$> (return $ case b of
                             BoolTrue -> True
                             BoolFalse ->  False)

evalList :: ListExp -> BerlinResult
evalList (SListExp list) = do
  mapM (\(SListHlp exp) -> eval exp) list >>= (\l -> return $ ListVal l)

evalExpTer :: ExpT -> BerlinResult
evalExpTer (ConExpT c) = evalConstant c
evalExpTer (VarExpT (BIdent ident)) = getFromEnv ident
evalExpTer (AlgExpT (AlgTypeIdent ident)) = getFromEnv ident
evalExpTer (ListExpT list) = evalList list

getFromEnv :: String -> BerlinResult
getFromEnv ident = do
  env <- ask
  case M.lookup ident env of
    Just v -> return $ v
    _ -> throwError $ IdentifierNotFound ident

evalExp :: Exp -> BerlinResult
evalExp (ETer ter) = evalExpTer ter
evalExp (EAdd exp1 exp2) = arithmeticEval (+) (.+) exp1 exp2
evalExp (ESub exp1 exp2) = arithmeticEval (-) (.-) exp1 exp2
evalExp (EMul exp1 exp2) = arithmeticEval (*) (.*) exp1 exp2
evalExp (EDiv exp1 exp2) = arithmeticEval div (\_ _ -> undefined) exp1 exp2

evalExp (EGr exp1 exp2) = booleanEval (>) exp1 exp2
evalExp (EGrEq exp1 exp2) = booleanEval (>=) exp1 exp2
evalExp (EEq exp1 exp2) = booleanEval (==) exp1 exp2
evalExp (ELe exp1 exp2) = booleanEval (<) exp1 exp2
evalExp (ELeEq exp1 exp2) = booleanEval (<=) exp1 exp2

evalExp (EListExp) = return $ ListVal []
evalExp (CListExp head tail) = do
  h <- evalExp head
  t <- evalExp tail
  case t of
    ListVal l -> return $ ListVal (h:l)
    _ -> throwError InvalidTypes

evalExp e@(CondExp expC expT expF) = do
  eexpC <- eval expC
  case eexpC of
    BoolVal b -> eval $ if b then expT else expF
    _ -> throwError $ ContextRequiresBool e expC
evalExp (LetExp decs exp) = do
  env <- ask
  newEnv <- evalDecTs env decs
  local (\env -> newEnv) (evalExp exp)
evalExp (MatchExp exp ps) = do
    eexp <- eval exp
    case applyToFirstMatching (evalAbsPatern eexp) ps of
      Just res -> res
      Nothing -> throwError $ PatternMatchFailed exp ps


evalExp e@(AppExp exp1 exp2) = do
  eexp1 <- eval exp1
  eexp2 <- eval exp2
  case eexp1 of
    (FunVal f) -> f eexp2
    _ -> throwError $ ApplicationFailure e
evalExp e@(InfAppExp exp1 ident exp2) =
  evalExp $ toPlainApplication e

evalExp (LambdaExp (BIdent ident) _ exp) = do
  env <- ask
  return $ FunVal $ \exp1 -> local (M.union (M.insert ident exp1 env)) (evalExp exp)


evalAbsPatern :: ExpVal -> CasePattern -> Maybe BerlinResult
evalAbsPatern eexp absPatern = do
  case absPatern of
    TypePattern p e -> matchPattern eexp (evalPattern p, e)

matchPattern :: ExpVal -> (PatternVal, Exp) -> Maybe BerlinResult
matchPattern val (p, e) = evalMatchPattern val p >>=
                          \env -> return $ local (M.union env) (eval e)


applyToFirstMatching :: (a -> Maybe b) -> [a] -> Maybe b
applyToFirstMatching app list =
  case (catMaybes $ map app list) of
    h:_ -> Just h
    [] -> Nothing

arithmeticEval :: (Integer -> Integer -> Integer) ->
                  (ComplexInt -> ComplexInt -> ComplexInt) ->
                  Exp -> Exp -> BerlinResult
arithmeticEval plain complex exp1 exp2 =
  do
    eexp1 <- eval exp1
    eexp2 <- eval exp2
    case (eexp1, eexp2) of
      (IntVal x, IntVal y) -> return $ IntVal $ plain x y
      (ComplexIntVal x, ComplexIntVal y) ->
        return $ ComplexIntVal $ complex x y
      _ -> throwError InvalidTypes

booleanEval :: (Integer -> Integer -> Bool) -> Exp -> Exp -> BerlinResult
booleanEval f exp1 exp2 =
  do
    eexp1 <- eval exp1
    eexp2 <- eval exp2
    case (eexp1, eexp2) of
      (IntVal x, IntVal y) -> return $ BoolVal $ f x y
      _ -> throwError InvalidTypes
