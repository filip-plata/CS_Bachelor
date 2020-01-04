r := Set with: $S -> 'aAf' with: $S -> 'BC'.
r add: $A -> 'BaB'.
r add: $B -> 'C'.
r add: $B -> 'Dxx'.
r add: $C -> 'xBy'.
r add: $B -> ''.

p := ParserLL1 start: $S rules: r.

self assert: [p isNil].
