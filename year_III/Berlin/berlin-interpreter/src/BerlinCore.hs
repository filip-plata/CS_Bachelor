module BerlinCore where

import Control.Monad
import Control.Monad.State.Lazy
import Control.Monad.Except
import Control.Monad.Reader
import Control.Monad.Identity

import Data.List

import qualified Data.Map as M

import AbsGrammar

data TyBerlin = TyInt
              | TyChar
              | TyBool
              | TyComplex
              | TyFun TyBerlin TyBerlin
              | TyAlg String
              | TyList TyBerlin
              | TyVar String
              deriving Eq

data ExpVal = IntVal Integer
            | CharVal Char
            | BoolVal Bool
            | ComplexIntVal ComplexInt
            | FunVal (ExpVal -> BerlinResult)
            | AlgVal String [ExpVal]
            | ListVal [ExpVal]

data PatternVal = ListPattern PatternVal PatternVal | ListEmpty
                | AlgPattern AlgTypeIdent [PatternVal]
                | Dummy | VarPattern String deriving (Show, Eq)

data BerlinRuntimeError = InvalidTypes
                        | IdentifierNotFound String
                        | PatternMatchFailed Exp [CasePattern]
                        | ContextRequiresBool Exp Exp
                        | ApplicationFailure Exp
                        | IntArgumentRequired

data ComplexInt = Complex Integer Integer deriving Eq
type Env = M.Map String ExpVal

type Interpreter a = ReaderT Env (Except BerlinRuntimeError) a

type BerlinEnv = Interpreter Env
type BerlinResult = Interpreter ExpVal
type BerlinPattern = Interpreter PatternVal


evalMatchPattern :: ExpVal -> PatternVal -> Maybe Env
evalMatchPattern exp (VarPattern ident) = return $ M.singleton ident exp
evalMatchPattern _ Dummy = return $ M.empty
evalMatchPattern (ListVal (h:t)) (ListPattern x pt) =
  do
    tail <- evalMatchPattern (ListVal t) pt
    head <- evalMatchPattern h x
    return $ M.union tail head
evalMatchPattern (ListVal []) ListEmpty = return $ M.empty
evalMatchPattern (AlgVal name values)
                 p@(AlgPattern (AlgTypeIdent pName) subPatterns) =
  if name /= pName then Nothing else
  foldM (\env (pt, val) -> evalMatchPattern val pt >>=
                            \env1 -> return $ M.union env env1)
          M.empty (zip subPatterns values)
evalMatchPattern _ _ = Nothing

evalPattern :: PatternExp -> PatternVal
evalPattern (PatternList head tail) = ListPattern (evalPattern head) (evalPattern tail)
evalPattern PatternListEm = ListEmpty
evalPattern (PatternExpVar (BIdent ident)) = VarPattern ident
evalPattern PatternDummy = Dummy
evalPattern (PatternExpl ident patterns) =
  AlgPattern ident $ map evalPattern patterns


evalTypeSpecifier :: Type_specifier -> TyBerlin
evalTypeSpecifier IntType = TyInt
evalTypeSpecifier BoolType = TyBool
evalTypeSpecifier ComType = TyComplex
evalTypeSpecifier CharType = TyChar
evalTypeSpecifier (AlgType (AlgTypeIdent ident)) = TyAlg ident
evalTypeSpecifier (PolyType (TypeVariable s)) = TyVar s
evalTypeSpecifier (FunType ts1 ts2) = TyFun (evalTypeSpecifier ts1) (evalTypeSpecifier ts2)
evalTypeSpecifier (ListType ts) = TyList $ evalTypeSpecifier ts


-- Builtin
fromIntToComplex :: ExpVal -> BerlinResult
fromIntToComplex (IntVal i) = return $ ComplexIntVal $ fromIntegerC i
fromIntToComplex _ = throwError IntArgumentRequired


instance Show ExpVal where
    show (IntVal i) = show i
    show (CharVal c) = show c
    show (BoolVal b) = show b
    show (ComplexIntVal c) = show c
    show (ListVal l) = "[" ++ intercalate ", " (map show l)  ++ "]"
    show (AlgVal ident exps) =
      show ident ++ " " ++ intercalate " " (map show exps)
    show (FunVal _) = "no instance of show for functional values"

instance Show TyBerlin where
    show TyInt = "Int"
    show TyBool = "Bool"
    show TyChar = "Char"
    show TyComplex = "Complex"
    show (TyList t) = "[" ++ show t ++ "]"
    show (TyFun t1 t2) = show t1 ++ "->" ++ show t2
    show (TyAlg t) = t
    show (TyVar var) = var

instance Show BerlinRuntimeError where
    show InvalidTypes = "type checker failure - internal error"
    show (IdentifierNotFound ident) = "not found " ++ show ident

instance Show ComplexInt where
    show (Complex a b) = show a ++ " + (" ++ show b ++ "i)"

fromIntegerC n = Complex n 0
negateC (Complex a b) = (Complex (-a) (-b))
(.+) (Complex a b) (Complex c d) = Complex (a + c) (b + d)
(.*) (Complex a b) (Complex c d) = Complex (a*c - b*d) (a*d + b*c)
(.-) a b = a .+ (negateC b)


toPlainApplication :: Exp -> Exp
toPlainApplication (InfAppExp exp1 ident exp2) =
  AppExp (AppExp (ETer (VarExpT ident)) exp1) exp2

desugarToLambda1 (ArgDef ident ty) exp = LambdaExp ident ty exp

desugarToLambda :: [Arg] -> Exp -> Exp
desugarToLambda [arg] exp = desugarToLambda1 arg exp
desugarToLambda (arg:args) exp = desugarToLambda1 arg $ desugarToLambda args exp

getPattern :: CasePattern -> PatternExp
getPattern (TypePattern p _) = p

getExp :: CasePattern -> Exp
getExp (TypePattern _ exp) = exp
