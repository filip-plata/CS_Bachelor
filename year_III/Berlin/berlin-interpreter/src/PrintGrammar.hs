{-# OPTIONS_GHC -fno-warn-incomplete-patterns #-}
module PrintGrammar where

-- pretty-printer generated by the BNF converter

import AbsGrammar
import Data.Char


-- the top-level printing method
printTree :: Print a => a -> String
printTree = render . prt 0

type Doc = [ShowS] -> [ShowS]

doc :: ShowS -> Doc
doc = (:)

render :: Doc -> String
render d = rend 0 (map ($ "") $ d []) "" where
  rend i ss = case ss of
    "["      :ts -> showChar '[' . rend i ts
    "("      :ts -> showChar '(' . rend i ts
    "{"      :ts -> showChar '{' . new (i+1) . rend (i+1) ts
    "}" : ";":ts -> new (i-1) . space "}" . showChar ';' . new (i-1) . rend (i-1) ts
    "}"      :ts -> new (i-1) . showChar '}' . new (i-1) . rend (i-1) ts
    ";"      :ts -> showChar ';' . new i . rend i ts
    t  : "," :ts -> showString t . space "," . rend i ts
    t  : ")" :ts -> showString t . showChar ')' . rend i ts
    t  : "]" :ts -> showString t . showChar ']' . rend i ts
    t        :ts -> space t . rend i ts
    _            -> id
  new i   = showChar '\n' . replicateS (2*i) (showChar ' ') . dropWhile isSpace
  space t = showString t . (\s -> if null s then "" else (' ':s))

parenth :: Doc -> Doc
parenth ss = doc (showChar '(') . ss . doc (showChar ')')

concatS :: [ShowS] -> ShowS
concatS = foldr (.) id

concatD :: [Doc] -> Doc
concatD = foldr (.) id

replicateS :: Int -> ShowS -> ShowS
replicateS n f = concatS (replicate n f)

-- the printer class does the job
class Print a where
  prt :: Int -> a -> Doc
  prtList :: Int -> [a] -> Doc
  prtList i = concatD . map (prt i)

instance Print a => Print [a] where
  prt = prtList

instance Print Char where
  prt _ s = doc (showChar '\'' . mkEsc '\'' s . showChar '\'')
  prtList _ s = doc (showChar '"' . concatS (map (mkEsc '"') s) . showChar '"')

mkEsc :: Char -> Char -> ShowS
mkEsc q s = case s of
  _ | s == q -> showChar '\\' . showChar s
  '\\'-> showString "\\\\"
  '\n' -> showString "\\n"
  '\t' -> showString "\\t"
  _ -> showChar s

prPrec :: Int -> Int -> Doc -> Doc
prPrec i j = if j<i then parenth else id


instance Print Integer where
  prt _ x = doc (shows x)


instance Print Double where
  prt _ x = doc (shows x)


instance Print Ident where
  prt _ (Ident i) = doc (showString ( i))


instance Print AlgTypeIdent where
  prt _ (AlgTypeIdent i) = doc (showString ( i))


instance Print BIdent where
  prt _ (BIdent i) = doc (showString ( i))


instance Print TypeVariable where
  prt _ (TypeVariable i) = doc (showString ( i))



instance Print Constant where
  prt i e = case e of
    ConNum numeral -> prPrec i 0 (concatD [prt 0 numeral])
    ConChar c -> prPrec i 0 (concatD [prt 0 c])
    ConBool boolean -> prPrec i 0 (concatD [prt 0 boolean])

instance Print Boolean where
  prt i e = case e of
    BoolTrue -> prPrec i 0 (concatD [doc (showString "True")])
    BoolFalse -> prPrec i 0 (concatD [doc (showString "False")])

instance Print Numeral where
  prt i e = case e of
    NumInt n -> prPrec i 0 (concatD [prt 0 n])
    NumComplex complex -> prPrec i 0 (concatD [prt 0 complex])

instance Print Complex where
  prt i e = case e of
    IntComp n -> prPrec i 0 (concatD [prt 0 n, doc (showString "i")])

instance Print Type_specifier where
  prt i e = case e of
    FunType typespecifier1 typespecifier2 -> prPrec i 0 (concatD [prt 0 typespecifier1, doc (showString "->"), prt 1 typespecifier2])
    ListType typespecifier -> prPrec i 0 (concatD [doc (showString "["), prt 0 typespecifier, doc (showString "]")])
    IntType -> prPrec i 1 (concatD [doc (showString "Int")])
    BoolType -> prPrec i 1 (concatD [doc (showString "Bool")])
    CharType -> prPrec i 1 (concatD [doc (showString "Char")])
    ComType -> prPrec i 1 (concatD [doc (showString "Complex")])
    AlgType algtypeident -> prPrec i 1 (concatD [prt 0 algtypeident])
    PolyType typevariable -> prPrec i 1 (concatD [prt 0 typevariable])
  prtList _ [] = (concatD [])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString " "), prt 0 xs])
instance Print PatternExp where
  prt i e = case e of
    PatternExpVar bident -> prPrec i 0 (concatD [prt 0 bident])
    PatternExpl algtypeident patternexps -> prPrec i 0 (concatD [prt 0 algtypeident, prt 1 patternexps])
    PatternList patternexp1 patternexp2 -> prPrec i 0 (concatD [prt 0 patternexp1, doc (showString ":"), prt 1 patternexp2])
    PatternListEm -> prPrec i 0 (concatD [doc (showString "[]")])
    PatternDummy -> prPrec i 0 (concatD [doc (showString "_")])
  prtList 1 [] = (concatD [])
  prtList 1 [x] = (concatD [prt 1 x])
  prtList 1 (x:xs) = (concatD [prt 1 x, doc (showString " "), prt 1 xs])
instance Print ListExp where
  prt i e = case e of
    SListExp hlistexps -> prPrec i 0 (concatD [doc (showString "["), prt 0 hlistexps, doc (showString "]")])

instance Print HListExp where
  prt i e = case e of
    SListHlp exp -> prPrec i 0 (concatD [prt 0 exp])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ","), prt 0 xs])
instance Print Exp where
  prt i e = case e of
    LetExp decters exp -> prPrec i 0 (concatD [doc (showString "let"), prt 0 decters, doc (showString "in"), prt 0 exp])
    CondExp exp1 exp2 exp3 -> prPrec i 0 (concatD [doc (showString "if"), prt 1 exp1, doc (showString "then"), prt 1 exp2, doc (showString "else"), prt 1 exp3])
    MatchExp exp casepatterns -> prPrec i 0 (concatD [doc (showString "case"), prt 1 exp, doc (showString "of"), prt 0 casepatterns])
    AppExp exp1 exp2 -> prPrec i 1 (concatD [prt 1 exp1, prt 2 exp2])
    InfAppExp exp1 bident exp2 -> prPrec i 1 (concatD [prt 1 exp1, doc (showString "`"), prt 0 bident, doc (showString "`"), prt 2 exp2])
    LambdaExp bident typespecifier exp -> prPrec i 2 (concatD [doc (showString "\\"), prt 0 bident, doc (showString "<"), prt 0 typespecifier, doc (showString ">"), doc (showString "->"), prt 3 exp])
    EAdd exp1 exp2 -> prPrec i 3 (concatD [prt 3 exp1, doc (showString "+"), prt 4 exp2])
    ESub exp1 exp2 -> prPrec i 3 (concatD [prt 3 exp1, doc (showString "-"), prt 4 exp2])
    EMul exp1 exp2 -> prPrec i 4 (concatD [prt 4 exp1, doc (showString "*"), prt 5 exp2])
    EDiv exp1 exp2 -> prPrec i 4 (concatD [prt 4 exp1, doc (showString "/"), prt 5 exp2])
    ELeEq exp1 exp2 -> prPrec i 5 (concatD [prt 5 exp1, doc (showString "<="), prt 6 exp2])
    EGrEq exp1 exp2 -> prPrec i 5 (concatD [prt 5 exp1, doc (showString ">="), prt 6 exp2])
    ELe exp1 exp2 -> prPrec i 5 (concatD [prt 5 exp1, doc (showString "<"), prt 6 exp2])
    EGr exp1 exp2 -> prPrec i 5 (concatD [prt 5 exp1, doc (showString ">"), prt 6 exp2])
    EEq exp1 exp2 -> prPrec i 5 (concatD [prt 5 exp1, doc (showString "=="), prt 6 exp2])
    CListExp exp1 exp2 -> prPrec i 6 (concatD [prt 6 exp1, doc (showString ":"), prt 7 exp2])
    EListExp -> prPrec i 7 (concatD [doc (showString "[]")])
    ETer expt -> prPrec i 8 (concatD [prt 0 expt])

instance Print CasePattern where
  prt i e = case e of
    TypePattern patternexp exp -> prPrec i 0 (concatD [doc (showString "|"), prt 0 patternexp, doc (showString "->"), prt 1 exp])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString "|"), prt 0 xs])
instance Print ExpT where
  prt i e = case e of
    ConExpT constant -> prPrec i 0 (concatD [prt 0 constant])
    VarExpT bident -> prPrec i 0 (concatD [prt 0 bident])
    ListExpT listexp -> prPrec i 0 (concatD [prt 0 listexp])
    AlgExpT algtypeident -> prPrec i 0 (concatD [prt 0 algtypeident])

instance Print AlgTypeConsDec where
  prt i e = case e of
    AlgTypeDec algtypeident typespecifiers -> prPrec i 0 (concatD [prt 0 algtypeident, prt 0 typespecifiers])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString "|"), prt 0 xs])
instance Print Dec where
  prt i e = case e of
    DecAlgType algtypeident algtypeconsdecs -> prPrec i 0 (concatD [doc (showString "data"), prt 0 algtypeident, doc (showString ":="), prt 0 algtypeconsdecs])
    DecT decter -> prPrec i 0 (concatD [prt 0 decter])
  prtList _ [] = (concatD [])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ";;"), prt 0 xs])
instance Print DecTer where
  prt i e = case e of
    DecVal bident typespecifier exp -> prPrec i 0 (concatD [prt 0 bident, doc (showString ";"), prt 0 typespecifier, doc (showString ":="), prt 0 exp])
    DecFun bident args typespecifier exp -> prPrec i 0 (concatD [prt 0 bident, prt 0 args, doc (showString ";"), prt 0 typespecifier, doc (showString ":="), prt 0 exp])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ";"), prt 0 xs])
instance Print Arg where
  prt i e = case e of
    ArgDef bident typespecifier -> prPrec i 0 (concatD [prt 0 bident, doc (showString ":"), prt 0 typespecifier])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ","), prt 0 xs])
instance Print Module where
  prt i e = case e of
    Load id -> prPrec i 0 (concatD [doc (showString "load"), prt 0 id])
  prtList _ [] = (concatD [])
  prtList _ [x] = (concatD [prt 0 x])
  prtList _ (x:xs) = (concatD [prt 0 x, doc (showString ";;"), prt 0 xs])
instance Print Program where
  prt i e = case e of
    Main modules decs exp -> prPrec i 0 (concatD [prt 0 modules, prt 0 decs, doc (showString "main"), doc (showString ":="), prt 0 exp, doc (showString ";;")])


