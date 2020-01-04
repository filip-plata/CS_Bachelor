{--------------------------------------------------
 Semantyka denotacyjna jezyka TINY z deklaracjami zmiennych
---------------------------------------------------}

{----------------------------------------------
 Semantyka zapisana w jezyku Haskell.

 Po uruchomieniu interpretera Haskella (ghci)
 nalezy zaladowac plik Declarations.hs.
-----------------------------------------------}

import Data.Map

-- SYNTAX --

data Exp = NumExp Int
         | VarExp String
         | PlusExp Exp Exp
         | MinusExp Exp Exp
         | TimesExp Exp Exp

data BExp = TrueBExp
          | FalseBExp
          | LEqBExp Exp Exp
          | NotBExp BExp
          | AndBExp BExp BExp
          | EQExp Exp Exp

data VDecl = NoVDecl
           | SingleVDecl (String, Int) VDecl

data Stmt = SkipStmt
          | AssStmt String Exp
          | SeqStmt Stmt Stmt
          | IfStmt BExp Stmt Stmt
          | WhileStmt BExp Stmt
          | BlockStmt VDecl Stmt
          | CatchStmt BExp Stmt

-- AUXILIARY DOMAINS --

type Loc = Int
type Store = Map Loc Int
type VEnv = Map String Loc
type SkipIns = Bool
type SemVal = Store


-- UTILS --

newloc :: Store -> Int
newloc s = size s

evalStore :: Stmt -> Store
evalStore = undefined


-- SEMANTIC FUNCTIONS --

semE :: Exp -> VEnv -> Store -> Int
semE (NumExp n) rho s = n
semE (VarExp v) rho s = s ! (rho ! v)
semE (PlusExp e1 e2) rho s = (semE e1 rho s) + (semE e2 rho s)
semE (MinusExp e1 e2) rho s = (semE e1 rho s) - (semE e2 rho s)
semE (TimesExp e1 e2) rho s = (semE e1 rho s) * (semE e2 rho s)

semB :: BExp -> VEnv -> Store -> Bool
semB TrueBExp rho s = True
semB FalseBExp rho s = False
semB (LEqBExp e1 e2) rho s = (semE e1 rho s) <= (semE e2 rho s)
semB (NotBExp b) rho s = not (semB b rho s)
semB (AndBExp b1 b2) rho s = (semB b1 rho s) && (semB b2 rho s)
semB (EQExp e1 e2) rho s = (semE e1 rho s) == (semE e2 rho s)

-- this not quite the same as in the lecture's slides:
-- newly declared variables are initialized to zero.
semD :: VDecl -> VEnv -> Store -> (VEnv,Store)
semD NoVDecl rho s = (rho,s)
semD (SingleVDecl (v, value) d) rho s =
        let l = newloc s
        in semD d (insert v l rho) (insert l value s)

semS :: Stmt -> VEnv  -> Store -> SemVal
semS = undefined

-- TEST: BASIC CATCH --

{--
begin
  var x = 0;
  catch x=2 in
    while true do
      x := x+1;
end

/* x --> 2 */
--}

basicCatchStmt :: Stmt
basicCatchStmt =
  BlockStmt
    (SingleVDecl ("x", 0) NoVDecl)
    (CatchStmt
      (EQExp (VarExp "x") (NumExp 2))
      (WhileStmt
        TrueBExp
        (AssStmt "x" (PlusExp (VarExp "x") (NumExp 1)))
      )
    )

testBasicCatchStmt :: Bool
testBasicCatchStmt =
  let s = evalStore basicCatchStmt in
   s ! 0 == 2


-- TEST: CATCH SKIP INSTRUCTIONS --

{--
begin
  var x = 0; var y = 13;
  catch x=y in
    while true do
      x := x+1;
      y := y-1;
end
--}

catchSkipInstructionStmt :: Stmt
catchSkipInstructionStmt =
  BlockStmt
    (SingleVDecl ("x", 0) (SingleVDecl ("y", 13) NoVDecl))
    (CatchStmt
      (EQExp (VarExp "x") (VarExp "y"))
      (WhileStmt
        TrueBExp
        (SeqStmt
          (AssStmt "x" (PlusExp (VarExp "x") (NumExp 1)))
          (AssStmt "y" (PlusExp (VarExp "y") (NumExp (-1))))
        )
      )
    )

testCatchSkipInstructionStmt :: Bool
testCatchSkipInstructionStmt =
  let s = evalStore catchSkipInstructionStmt in
  ((s ! 0) == 7) && ((s ! 1) == 7)


-- TEST: EXAMPLE TEST --
{--
begin
  var x=0; var y=0; var z=0;
  x := 1;
  y := 10;
  catch x=y in
    while true do begin
      var x=0;
      catch x=y in
        while true do
          x := x+1; z := z+1;
      y := y-1;
    end
end
--}

exampleTestStmt :: Stmt
exampleTestStmt =
  BlockStmt
   (SingleVDecl ("x", 1) (SingleVDecl ("y", 10) (SingleVDecl ("z", 0) NoVDecl)))
   (CatchStmt
    (EQExp (VarExp "x") (VarExp "y"))
    (WhileStmt
      TrueBExp
      (BlockStmt
        (SingleVDecl ("x", 0) NoVDecl)
        (SeqStmt
          (CatchStmt
            (EQExp (VarExp "x") (VarExp "y"))
            (WhileStmt
              TrueBExp
              (SeqStmt
                (AssStmt "x" (PlusExp (VarExp "x") (NumExp 1)))
                (AssStmt "z" (PlusExp (VarExp "z") (NumExp 1)))
              )
            )
          )
          (AssStmt "y" (PlusExp (VarExp "y") (NumExp (-1))))
        )
      )
    )
   )

testExampleTestStmt :: Bool
testExampleTestStmt =
  let s = evalStore exampleTestStmt in
  (s ! 2 == 45) && (s ! 1 == 1)


-- TEST: DECLARATION CHEKED BY CATCH --
{--
begin
  var x=0; var y=5;
  catch x=1 in
    begin
      var x=1;
      y := 10;
    end
end
--}

declarationCheckedByCatch :: Stmt
declarationCheckedByCatch =
  BlockStmt
    (SingleVDecl ("x", 0) (SingleVDecl ("y", 5) NoVDecl))
    (CatchStmt
      (EQExp (VarExp "x") (NumExp 1))
      (BlockStmt
        (SingleVDecl ("x", 1) NoVDecl)
        (AssStmt "x" (NumExp 10))
      )
    )

testDeclarationCheckedByCatch :: Bool
testDeclarationCheckedByCatch =
  let s = evalStore declarationCheckedByCatch in
  s ! 1 == 5

-- EXAMPLE: BASIC SCOPING --

{--
begin
  var x;
  var y
  begin
    var x
    x := 7;
    y := x+1
  end;
  x := y+1
end
--}

fooStmt :: Stmt
fooStmt =
  BlockStmt
    (SingleVDecl ("x", 0) (SingleVDecl ("y", 0) NoVDecl))
    (SeqStmt
      (BlockStmt
        (SingleVDecl ("x", 0) NoVDecl)
        (SeqStmt
          (AssStmt "x" (NumExp 7))
          (AssStmt "y" (PlusExp (VarExp "x") (NumExp 1)))
        )
      )
      (AssStmt "x" (PlusExp (VarExp "y") (NumExp 1)))
    )

{--
After running this program, locations 0 to 2 will hold values 9,8,7.

An example session that checks this:
 $ghci
 GHCi, version 7.10.3:
 Prelude> :load Declarations.hs
 [1 of 1] Compiling Main             ( Declarations.hs, interpreted )
 Ok, modules loaded: Main.
 *Main> let s = semS fooStmt empty empty
 *Main> s ! 0
 9
 *Main> s ! 1
 8
 *Main> s ! 2
 7
--}
