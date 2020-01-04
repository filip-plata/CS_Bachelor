r := Set with: $S -> 'aSa' with: $S -> 'bSb' with: $S -> 'b' with: $S -> 'a'.

p := ParserLL1 start: $X rules: r.

self assert: [ p isNil ].
