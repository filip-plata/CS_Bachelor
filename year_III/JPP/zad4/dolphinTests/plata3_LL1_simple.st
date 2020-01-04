r := Set with: $S -> 'AaAb' with: $S -> 'BbBa' with: $A -> '' with: $B -> ''.
p := ParserLL1 start: $S rules: r.

pa := p react: $a.
pb := p react: $b.

self assert: [p accept not].
self assert: [p predict asSet equals: 'ab' asSet].
self assert: [pa predict asSet equals: 'b' asSet].
self assert: [pb predict asSet equals: 'a' asSet].
