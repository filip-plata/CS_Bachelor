r := Set with: $L -> 'La' with: $L -> ''.
l := ParserLL1 start: $L rules: r.
self assert: [l isNil].
