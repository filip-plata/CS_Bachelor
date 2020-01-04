FileStream fileIn: 'ParserLL1.st'.

Object subclass: Assert [

]

r := OrderedCollection with: $A -> 'xAz'.

a := ParserLL1 start: $A rules: r.
a reject printNl.

b := ParserLL1 start: $B rules: r.
b reject printNl.

r add: $A -> 'B'.
r add: $B -> 'yB'.
r add: $B -> ''.

c := ParserLL1 start: $B rules: r.
c accept printNl.

d := c react: $x.
d reject printNl.

e := ParserLL1 start: $A rules: r.

r add: $B -> 'y'.

f := ParserLL1 start: $A rules: r.
f isNil printNl.
e accept printNl.
((e predict asSet) = ('xy' asSet)) printNl.

g := e react: $x.
g accept not printNl.
g reject not printNl.
((g predict asSet) = ('xyz' asSet)) printNl.

h := g react: $z.
h accept printNl.
h predict isEmpty printNl.

r := Set with: $S -> '(S)S' with: $S -> ''.
a := ParserLL1 start: $S rules: r.
b := '((()))(()(()))()' inject: a into: [:q :c | q react: c].
b accept printNl.

r := (OrderedCollection new)
    add: #Expression -> #(#Term #ExpressionTail);
    add: #ExpressionTail -> #();
    add: #ExpressionTail -> #(#+ #Term #ExpressionTail);
    add: #Term -> #(#Factor #TermTail);
    add: #TermTail -> #();
    add: #TermTail -> #(#* #Factor #TermTail);
    add: #Factor -> #(#n);
    add: #Factor -> #('(' #Expression ')');
    yourself.

p := ParserLL1 start: #Expression rules: r.
q := OrderedCollection with: '' -> p.
v := OrderedCollection new.
[q isEmpty] whileFalse:
    [| x s t |
    x := q removeFirst.
    s := x key.
    t := x value.
    t accept ifTrue: [v add: s].
    s size < 5 ifTrue:
        [t predict asSortedCollection do:
            [:each | q add: (s , each) -> (t react: each)]]].
w := #('n' '(n)' 'n*n' 'n+n' '((n))' '(n)*n' '(n)+n' '(n*n)' '(n+n)' 'n*(n)' 'n*n*n' 'n*n+n' 'n+(n)' 'n+n*n' 'n+n+n').

(v asArray = w) printNl.
