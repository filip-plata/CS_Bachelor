r := Set with: $S -> 'xyza'.
p := ParserLL1 start: $S rules: r.

px := p react: $x.
pxy := px react: $y.
pxyz := pxy react: $z.
pxyza := pxyz react: $a.

self assert: [p reject not].
self assert: [p accept not].
self assert: [pxyza accept].
self assert: [pxyza reject not].

self assert: [p predict asSet equals: 'x' asSet].
self assert: [px predict asSet equals: 'y' asSet].
self assert: [pxy predict asSet equals: 'z' asSet].
self assert: [pxyz predict asSet equals: 'a' asSet].
self assert: [pxyza predict asSet equals: '' asSet].
