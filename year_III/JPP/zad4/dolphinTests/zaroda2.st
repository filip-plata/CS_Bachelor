r := Set with: $S -> '(S)S' with: $S -> ''.
a := ParserLL1 start: $S rules: r.
b := '((()))(()(()))()' inject: a into: [:q :c | q react: c].
self assert: [b accept].
