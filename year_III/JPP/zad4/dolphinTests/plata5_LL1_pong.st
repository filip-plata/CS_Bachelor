r := Set with: $S -> 'aSb' with: $S -> ''.
p := ParserLL1 start: $S rules: r.

pa := p react: $a.
pab := pa react: $b.

self assert: [pa predict asSet equals: 'ab' asSet].
self assert: [pab accept].
self assert: [pab reject not].
self assert: [pab predict asSet equals: '' asSet].
