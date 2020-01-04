r := OrderedCollection with: $A -> 'xAz'.
a := ParserLL1 start: $A rules: r.
self assert: [a reject].
b := ParserLL1 start: $B rules: r.
self assert: [b reject].
r add: $A -> 'B'.
r add: $B -> 'yB'.
r add: $B -> ''.
c := ParserLL1 start: $B rules: r.
self assert: [c accept].
d := c react: $x.
self assert: [d reject].
e := ParserLL1 start: $A rules: r.
r add: $B -> 'y'.
f := ParserLL1 start: $A rules: r.
self assert: [f isNil].
self assert: [e accept].
self assert: [e predict asSet equals: 'xy' asSet].
g := e react: $x.
self assert: [g accept not].
self assert: [g reject not].
self assert: [g predict asSet equals: 'xyz' asSet].
h := g react: $z.
self assert: [h accept].
self assert: [h predict isEmpty].
