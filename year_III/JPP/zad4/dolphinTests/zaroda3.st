r := (OrderedCollection new)
    add: #Expression -> #(Term ExpressionTail);
    add: #ExpressionTail -> #();
    add: #ExpressionTail -> #(+ Term ExpressionTail);
    add: #Term -> #(Factor TermTail);
    add: #TermTail -> #();
    add: #TermTail -> #(* Factor TermTail);
    add: #Factor -> #(n);
    add: #Factor -> #('(' Expression ')');
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
self assert: [v asArray = w].
