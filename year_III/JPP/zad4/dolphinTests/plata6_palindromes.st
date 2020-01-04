r := Set with: $S -> 'aSa' with: $S -> 'bSb' with: $S -> 'b' with: $S -> 'a'.
r add: $S -> ''.

p := ParserLL1 start: $S rules: r.

self assert: [ p isNil ].
