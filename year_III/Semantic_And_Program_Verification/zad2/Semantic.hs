{--------------------------------------------------
 Semantyka denotacyjna jezyka TINY z deklaracjami zmiennych
---------------------------------------------------}

{----------------------------------------------
 Semantyka zapisana w jezyku Haskell.

 Po uruchomieniu interpretera Haskella (ghci)
 nalezy zaladowac plik Declarations.hs.
-----------------------------------------------}
module Semantic where

import Data.Map

-- SYNTAX --
type Fun = (String, String, Stmt)

data Exp = NumExp Int
         | VarExp String
         | PlusExp Exp Exp
         | MinusExp Exp Exp
         | TimesExp Exp Exp
         | CallFun Fun Exp

data BExp = TrueBExp
          | FalseBExp
          | LEqBExp Exp Exp
          | NotBExp BExp
          | AndBExp BExp BExp
          | EQExp Exp Exp

data Decl = VDecl String
           | NoDecl
           | FunDecl Fun
           | SeqDecl Decl Decl

data Stmt = SkipStmt
          | AssStmt String Exp
          | SeqStmt Stmt Stmt
          | IfStmt BExp Stmt Stmt
          | WhileStmt BExp Stmt
          | BlockStmt Decl Stmt

-- AUXILIARY DOMAINS --

type Loc = Int
type Store = Map Loc Int
type VEnv = Map String Loc
type PEnv = Map String Fun
type SkipIns = Bool
type SemVal = Store

-- UTILS --

newloc :: Store -> Int
newloc s = size s

evalStmt :: Stmt -> SemVal
evalStmt i = semS i empty empty empty

evalStore :: Stmt -> Store
evalStore = evalStmt


-- SEMANTIC FUNCTIONS --

semE :: Exp -> VEnv -> PEnv -> Store -> Int
semE (NumExp n) rho p s = n
semE (VarExp v) rho p s = s ! (rho ! v)
semE (PlusExp e1 e2) rho p s = (semE e1 rho p s) + (semE e2 rho p s)
semE (MinusExp e1 e2) rho p s = (semE e1 rho p s) - (semE e2 rho p s)
semE (TimesExp e1 e2) rho p s = (semE e1 rho p s) * (semE e2 rho p s)
semE (CallFun (f, x, i) e) rho p s =
  let n = semE e rho p s in
  let l1 = newloc s  in -- for x
  let l2 = newloc s in -- for f return value
  let s' = insert l1 n s in
  let s'' = insert l2 0 s' in
  n

semB :: BExp -> VEnv -> PEnv -> Store -> Bool
semB TrueBExp rho p s = True
semB FalseBExp rho p s = False
semB (LEqBExp e1 e2) rho p s = (semE e1 rho s) <= (semE e2 rho s)
semB (NotBExp b) rho p s = not (semB b rho s)
semB (AndBExp b1 b2) rho s = (semB b1 rho s) && (semB b2 rho s)
semB (EQExp e1 e2) rho s = (semE e1 rho s) == (semE e2 rho s)

-- this not quite the same as in the lecture's slides:
-- newly declared variables are initialized to zero.
semD :: Decl -> VEnv -> PEnv -> Store -> (VEnv, PEnv, Store)
semD NoDecl rho p s = (rho, p, s)
semD (VDecl v) rho p s =
        let l = newloc s
        in ((insert v l rho), p, (insert l 0 s))
semD (FunDecl (f, x, i)) rho p s = (rho, insert f (f, x, i) p, s)
semD (SeqDecl d1 d2) rho s =
    let (rho', s') = semD d1 rho s in
    semD d2 rho' s'

semS :: Stmt -> VEnv -> PEnv -> Store -> SemVal
semS SkipStmt rho p s = s
semS (AssStmt v e) rho p s =
       let n = semE e rho p s in
       insert (rho ! v) n s
semS (SeqStmt i1 i2) rho p s =
       let semSi1 = semS i1 rho p s in
       semS i2 rho p semSi1
semS (IfStmt b i1 i2) rho p s =
       if semB b rho s
       then semS i1 rho p s
       else semS i2 rho p s
semS (WhileStmt b i) rho p s =
       if semB b rho p s
       then let s' = semS i rho p s in
       semS (WhileStmt b i) rho p s'
       else s
semS (BlockStmt vd i) rho p s =
       let (rho1,s1) = semD vd rho p s
       in semS i rho1 p- s1
