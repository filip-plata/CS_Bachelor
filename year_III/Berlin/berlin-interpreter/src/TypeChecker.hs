module TypeChecker where

import Control.Monad.State
import Control.Monad.Except
import Control.Monad.Reader
import Control.Applicative

import Data.List

import Text.Printf

import qualified Data.Map as M

import PrintGrammar
import AbsGrammar

import Debug.Trace

import BerlinCore

data TypeError = Undefined
               | InvalidOperand Exp TyBerlin [TyBerlin] Context
               | ApplyError Context
               | ListElementsTypeMismatch [Exp]
               | IfDistinctTypes Exp Exp
               | DivisionByComplex Exp
               | DataConstructorNameTaken Context
               | NoContextInternalError
               | NameNotBound String Context
               | TypeUnificationError Context
               | CaseUnequalTypes Context
               | PatternMatchListExpected Context
               | PatternMatchAlgebraicTypeExpected String Context
               | EmptyCase Context
               | FunctionInvalidType String TyBerlin TyBerlin
               | DeclarationInvalidType String TyBerlin TyBerlin
               | ArithmeticInvalidTypes TyBerlin TyBerlin Context

class TypeCheck a where
  evalType :: a -> TCM TyBerlin

instance TypeCheck Program where
  evalType = evalTypeProgram


type NameSupply = Int
type Subst = M.Map String TyBerlin
data Constraint = TEQ TyBerlin TyBerlin
type Constraints = [Constraint]
type TypeEnv = M.Map String TyBerlin
data Context = CExp Exp | CDec Dec | CProg Program
type TCMEnv = (Maybe Context, TypeEnv)
type TCM a = ExceptT TypeError (StateT TcState (Reader TCMEnv)) a

data TcState = TcState {
    tcsNS       :: NameSupply,
    tcsSubst    :: Subst,
    constraints :: Constraints
}

tcmResolvedType :: TyBerlin -> TCM TyBerlin
tcmResolvedType tyVar@(TyVar name) = do
  s <- get
  case M.lookup name (tcsSubst s) of
    Just ty -> return ty
    Nothing -> return tyVar
tcmResolvedType ty = return ty

tcmFresh :: TCM String
tcmFresh = do
  s <- get
  put $ TcState ((tcsNS s) + 1) (tcsSubst s) (constraints s)
  return $ "t" ++ (show $ tcsNS s)

runTCM :: TCM a -> Either TypeError a
runTCM tcm = fst $ runReader (runStateT (runExceptT tcm)
             (TcState 0 M.empty [])) (Nothing, M.singleton "toComplex" (TyFun TyInt TyComplex))

evalTypeProgram :: Program -> TCM TyBerlin
evalTypeProgram p@(Main modules decs exp) = do
  decEnv <- evalDecsTypes decs
  ty <- localWithContext (CProg p) (M.union decEnv) (evalTypeExp exp)
  tcmResolvedType ty

evalDecType :: Dec -> TCM TypeEnv
evalDecType (DecT d) = evalDecTType d
evalDecType d@(DecAlgType ident cons) = do
  (_, env) <- ask
  envs <- mapM (evalAlgDecType ident) cons
  localContext (CDec d) $ foldM enlargeEnv env envs

evalAlgDecType :: AlgTypeIdent -> AlgTypeConsDec -> TCM TypeEnv
evalAlgDecType (AlgTypeIdent ident) d@(AlgTypeDec (AlgTypeIdent consIdent) types) = do
  consTy <- foldrM (liftF2 TyFun) (TyAlg ident) (map evalTypeSpecifier types)
  return $ M.singleton consIdent consTy

evalDecTType :: DecTer -> TCM TypeEnv
evalDecTType d@(DecFun (BIdent ident) args ty exp) =
  let tyFun = foldr TyFun (evalTypeSpecifier ty) (toTypes args) in
  let env = M.singleton ident tyFun in do
  evaluedType <- localWithContext (CDec (DecT d)) (M.union env)
                                 (evalTypeExp (desugarToLambda args exp))
  when (evaluedType /= tyFun) (throwError $ FunctionInvalidType ident tyFun evaluedType)
  return $ env

evalDecTType d@(DecVal (BIdent ident) ty exp) =
  let tyDec = evalTypeSpecifier ty in
  let env = M.singleton ident $ tyDec in do
  evaluedType <- localWithContext (CDec (DecT d)) (M.union env) (evalTypeExp exp)
  when (evaluedType /= tyDec) (throwError $ DeclarationInvalidType ident tyDec evaluedType)
  return $ env

evalTypeExp :: Exp -> TCM TyBerlin
evalTypeExp (ETer exp) = evalTypeExpTer exp

evalTypeExp e@(AppExp exp1 exp2) = do
  ty1 <- evalTypeExp exp1
  ty2 <- evalTypeExp exp2
  case ty1 of
    TyFun ty2 ty -> return ty
    _ -> throwError (ApplyError (CExp e))

evalTypeExp e@(InfAppExp exp1 ident exp2) =
  evalTypeExp $ toPlainApplication e

evalTypeExp e@(LambdaExp (BIdent ident) ty exp) =
  let t = evalTypeSpecifier ty in
  liftM (TyFun t) (localWithContext (CExp e) (M.union (M.singleton ident t))
  (evalTypeExp exp))

evalTypeExp e@(CondExp expC expT expF) =
  checkType expC TyBool >> do
  t1 <- evalTypeExp expT
  t2 <- evalTypeExp expF
  if t1 === t2 then return t1 else throwError $ IfDistinctTypes expT expF

evalTypeExp e@(MatchExp exp casePatterns) =
  let patterns = map (evalPattern . getPattern) casePatterns in
  let exps = map getExp casePatterns in
  localContext (CExp e) $ do
  eTy <- evalTypeExp exp
  resultTypes <- mapM (evalPatternExpType eTy) (zip patterns exps)
  when (null resultTypes) (throwContextualError EmptyCase)
  when (not $ all id (map ((===) (head resultTypes)) resultTypes))
    (throwError $ CaseUnequalTypes (CExp e))
  return $ head resultTypes

evalTypeExp e@(LetExp decs exp) = do
  decEnv <- localContext (CExp e) $ evalDecTsTypes decs
  localWithContext (CExp e) (M.union decEnv) (evalTypeExp exp)

evalTypeExp e@(EAdd exp1 exp2) =
  localContext (CExp e) $ checkArithmeticOperand exp1 exp2
evalTypeExp e@(ESub exp1 exp2) =
  localContext (CExp e) $ checkArithmeticOperand exp1 exp2
evalTypeExp e@(EMul exp1 exp2) =
  localContext (CExp e) $ checkArithmeticOperand exp1 exp2
evalTypeExp e@(EDiv exp1 exp2) = do
  t <- localContext (CExp e) $ checkArithmeticOperand exp1 exp2
  when (t /= TyInt) (throwError $ DivisionByComplex e)
  return t

evalTypeExp e@(ELeEq exp1 exp2) =
  localContext (CExp e) $ checkBoolOperand exp1 exp2
evalTypeExp e@(EGrEq exp1 exp2) =
  localContext (CExp e) $ checkBoolOperand exp1 exp2
evalTypeExp e@(ELe exp1 exp2) =
  localContext (CExp e) $ checkBoolOperand exp1 exp2
evalTypeExp e@(EGr exp1 exp2) =
  localContext (CExp e) $ checkBoolOperand exp1 exp2
evalTypeExp e@(EEq exp1 exp2) =
  localContext (CExp e) $ checkBoolOperand exp1 exp2


evalTypeExp EListExp = tcmFresh >>= (\n -> return $ TyList $ TyVar n)
evalTypeExp (CListExp head tail) = do
  tyT <- evalTypeExp tail
  case tyT of
    TyList ty -> checkType head ty >>= (\x -> return $ TyList x)
    _ -> throwError $ ListElementsTypeMismatch [head, tail]


evalTypeExpTer :: ExpT -> TCM TyBerlin
evalTypeExpTer (ConExpT c) = evalTypeConExpTer c
evalTypeExpTer (VarExpT (BIdent v)) = getFromEnv v
evalTypeExpTer e@(ListExpT l) = do
  state <- get
  l1 <- mapM evalTypeExp (toPlainList l)
  case nub l1 of
    [expTy] -> return $ TyList expTy
    _ -> throwError $ ListElementsTypeMismatch (toPlainList l)
evalTypeExpTer (AlgExpT (AlgTypeIdent ident)) = getFromEnv ident

evalTypeConExpTer :: Constant -> TCM TyBerlin
evalTypeConExpTer (ConNum num) = evalTypeNum num
evalTypeConExpTer (ConChar _) = return TyChar
evalTypeConExpTer (ConBool _) = return TyBool

evalTypeNum :: Numeral -> TCM TyBerlin
evalTypeNum (NumInt _) = return TyInt
evalTypeNum (NumComplex _) = return TyComplex

checkBoolOperand :: Exp -> Exp -> TCM TyBerlin
checkBoolOperand exp1 exp2 =
  checkType exp1 TyInt >>
  checkType exp2 TyInt >>
  return TyBool

checkArithmeticOperand :: Exp -> Exp -> TCM TyBerlin
checkArithmeticOperand exp1 exp2 = do
  t1 <- checkTypes exp1 armTy
  t2 <- checkTypes exp2 armTy
  case (t1, t2) of
    (TyInt, TyInt) -> return $ TyInt
    (TyComplex, TyComplex) -> return $ TyComplex
    _ -> throwContextualError $ ArithmeticInvalidTypes t1 t2
  where armTy = [TyInt, TyComplex]

evalPatternExpType :: TyBerlin -> (PatternVal, Exp) -> TCM TyBerlin
evalPatternExpType ty (p, e) = evalPatternEnv ty p >>= \patternEnv -> do
  env <- ask
  localPassContext (M.union patternEnv) (evalTypeExp e)


evalPatternEnv :: TyBerlin -> PatternVal -> TCM TypeEnv
evalPatternEnv _ Dummy = return $ M.empty
evalPatternEnv ty (VarPattern s) = return $ M.singleton s ty
evalPatternEnv ty ListEmpty = case ty of
  TyList _ -> return M.empty
  _ -> throwContextualError $ PatternMatchListExpected
evalPatternEnv ty (ListPattern a b) = case ty of
  t@(TyList ty) -> liftM2 M.union (evalPatternEnv ty a) (evalPatternEnv t b)
  _ -> throwContextualError $ PatternMatchListExpected
evalPatternEnv ty (AlgPattern (AlgTypeIdent ident) patterns) = do
  algCons <- getFromEnv ident
  algT <- getAlgConsResultType ident
  when (not $ ty == algT)
    (throwContextualError $ PatternMatchAlgebraicTypeExpected ident)
  evalPatterns algCons patterns


evalPatterns :: TyBerlin -> [PatternVal] -> TCM TypeEnv
evalPatterns (TyFun ty1 ty2) (h:t) =
  liftM2 M.union (evalPatternEnv ty1 h) (evalPatterns ty2 t)
evalPatterns _ [] = return M.empty
evalPatterns _ _ = throwError Undefined


checkTypes :: Exp -> [TyBerlin] -> TCM TyBerlin
checkTypes e types = evalTypeExp e >>=
  (\t1 -> if elem t1 types then return t1
          else throwContextualError $ InvalidOperand e t1 types)

checkType :: Exp -> TyBerlin -> TCM TyBerlin
checkType e t = evalTypeExp e >>=
  (\t1 -> if t1 === t then return t1
          else throwContextualError $ InvalidOperand e t1 [t])


evalDecsTypes :: [Dec] -> TCM TypeEnv
evalDecsTypes (dec:decs) = (evalDecType dec) >>= \d ->
  localWithContext (CDec dec) (M.union d) (evalDecsTypes decs)
evalDecsTypes [] = ask >>= \(_, e) -> return e

evalDecTsTypes :: [DecTer] -> TCM TypeEnv
evalDecTsTypes (dec:decs) = (evalDecTType dec) >>= \d ->
  localPassContext (M.union d) (evalDecTsTypes decs)
evalDecTsTypes [] = ask >>= \(_, e) -> return e

enlargeEnv :: TypeEnv -> TypeEnv -> TCM TypeEnv
enlargeEnv env1 env2 =
  let env = M.union env1 env2 in
  if M.size env == (M.size env1 + M.size env2)
  then return env
  else throwContextualError DataConstructorNameTaken

throwContextualError :: (Context -> TypeError) -> TCM a
throwContextualError error = ask >>= \(mExp, _) ->
  case mExp of
    Just contextExp -> throwError (error contextExp)
    Nothing -> throwError NoContextInternalError

toPlainList :: ListExp -> [Exp]
toPlainList (SListExp l) = map unwrap l

unwrap (SListHlp exp) = exp

foldrM :: Monad m => (a -> b -> m b) -> b -> [a] -> m b
foldrM f d []     = return d
foldrM f d (x:xs) = (\z -> f x z) =<< foldrM f d xs

liftF2 :: Monad m => (a -> b -> c) -> a -> b -> m c
liftF2 f a b = return $ f a b

getFromEnv :: String -> TCM TyBerlin
getFromEnv ident = do
  (_, env) <- ask
  case M.lookup ident env of
    Just ty -> return ty
    Nothing -> throwContextualError $ NameNotBound ident


localWithContext ::  Context -> (TypeEnv -> TypeEnv) -> TCM a -> TCM a
localWithContext exp trans = local (\(_, env) -> (Just exp, trans env))

localContext :: Context -> TCM a -> TCM a
localContext c r1 = localWithContext c id r1

localPassContext :: (TypeEnv -> TypeEnv) -> TCM a -> TCM a
localPassContext trans = local (\(mCon, env) -> (mCon, trans env))

toTypes :: [Arg] -> [TyBerlin]
toTypes args = map (\x -> case x of ArgDef _ t -> evalTypeSpecifier t) args

getAlgConsResultType :: String -> TCM TyBerlin
getAlgConsResultType s = getFromEnv s >>= getFunctionReturnType

getFunctionReturnType :: TyBerlin -> TCM TyBerlin
getFunctionReturnType (TyFun _ t1) = getFunctionReturnType t1
getFunctionReturnType t = return t

isTyList (TyList _) = True
isTyList _ = False

(===) :: TyBerlin -> TyBerlin -> Bool
(===) (TyVar _) _ = True
(===) _ (TyVar _) = True
(===) (TyList _) (TyList (TyVar _)) = True
(===) (TyList (TyVar _)) (TyList _) = True
(===) (TyFun t1 t2) (TyFun t3 t4) = t1 === t3 && t2 === t4
(===) a b = a == b


format :: [Char] -> [String] -> String
format s subst = s


instance Print Context where
  prt a (CExp e) = prt a e
  prt a (CDec d) = prt a d
  prt a (CProg p) = prt a p


instance Show TypeError where
    show Undefined = "Undefined type"
    show (DivisionByComplex e) = "Trying to perform division \
      \with complex numbers in: " ++ (printTree e)
    show (ApplyError e) = "Invalid first operand of application\
      \ in: " ++ (printTree e)
    show (InvalidOperand exp realType expected context) =
      printf "In expression: %s expected type of %s is one of: \
      \%s, but found %s" (printTree context) (printTree exp)
      (show expected) (show realType)
    show (ListElementsTypeMismatch l) = "Lists must have uniform \
      \type: " ++ printTree l
    show (IfDistinctTypes exp1 exp2) = printf "Distinct types of\
      \ %s and %s" (printTree exp1) (printTree exp2)
    show (NameNotBound name context) = printf "'%s' is not bound to\
      \ any value in: %s" name (printTree context)
    show (NoContextInternalError) = "Internal error of type checker.\n\
      \ No expression context was defined, but one was requested\
      \ when rising error"
    show (CaseUnequalTypes context) = printf "Cannot match types in\
      \ right hand of case match in: %s" (printTree context)
    show (TypeUnificationError context) = printf "Error when trying to\
      \ resolve types in %s" (printTree context)
    show (PatternMatchAlgebraicTypeExpected algTypeName context) =
      printf "Expected algebraic type: '%s' in : %s"
      algTypeName (printTree context)
    show (PatternMatchListExpected context) = printf "Expected list in:\
      \ %s" (printTree context)
    show (DataConstructorNameTaken context) = printf "Multiple usage of\
      \ algebraic constructor name near: %s" (printTree context)
    show (FunctionInvalidType ident expected real) = printf "In function '%s'\
      \ expected type: '%s', but found '%s'" (show ident) (show expected)
      (show real)
    show (DeclarationInvalidType ident expected real) = printf "In declaration of '%s'\
      \ expected type: '%s', but found '%s'" (show ident) (show expected)
      (show real)
    show (ArithmeticInvalidTypes ty1 ty2 context) = printf "Expected equal types in:\
      \ %s, but found '%s' and '%s'" (printTree context) (show ty1) (show ty2)
