Object subclass: Stack [
    | orderedCollection |
    Stack class >> empty [
      ^self new initialize.
    ]

    initialize [ orderedCollection := OrderedCollection new ]
    push: anElement [ orderedCollection addLast: anElement ]
    pushAll: elements [ orderedCollection addAll: elements ]
    pop [ | result |
      result := self top.
      self empty ifFalse: [ orderedCollection removeLast ].
      ^result.
    ]
    top [ self empty ifTrue: [ ^nil ] ifFalse: [ ^orderedCollection last ] ]
    empty [ ^orderedCollection size = 0 ]
    duplicate [ ^Stack empty pushAll: orderedCollection ]

    forAll: predicate [
      | result |
      result := true.
      orderedCollection do: [ :a | (predicate value: a)
                                   ifFalse: [ result := false ] ].
      ^result.
    ]

    printOn: stream [
      <category: 'printing'>
      orderedCollection printOn: stream
  ]
]

Object subclass: ParserState [
    | symbol input |
    ParserState class >> symbol: s input: i [
      ^self new symbol: s input: i.
    ]

    symbol: s input: i [
      symbol := s.
      input := i.
    ]

    symbol [ ^symbol ]
    input [ ^input ]

    hash [
      ^symbol hash + input hash.
    ]

    = aParserState [
      ^symbol = aParserState symbol & input = aParserState input.
    ]

    printOn: stream [
      <category: 'printing'>
      stream nextPutAll: 'Symbol: '.
      symbol printOn: stream.
      stream nextPutAll: ' Token: '.
      input printOn: stream.
  ]
]

Object subclass: ParserTable [
    | eps nonEmpty rules |
    ParserTable class >> empty: e nonEmpty: nE rules: parserRules [
      | rules_ |
      (rules_ := ParserTable constructTable: parserRules) = nil
        ifTrue: [^nil].
      ^self new empty: e nonEmpty: nE rules: rules_.
    ]

    ParserTable class >> constructTable: parserRules [
      | rules |
      rules := Dictionary new.
      parserRules do: [:r |
        "Check for multiple entries in parsing table"
        (rules includesKey: (r key)) ifTrue: [ ^nil ].
        rules at: (r key) put: (r value).
      ].
      ^rules.
    ]

    empty: e nonEmpty: nE rules: parserRules [
      eps := e.
      nonEmpty := nE.
      rules := parserRules.
    ]

    canDeriveToEmpty: s [ ^eps includes: s ]
    canDerive: s [ ^nonEmpty includes: s ]

    symbol: s input: i [
      s = i ifTrue: [ ^s ].
      ^rules at: (ParserState symbol: s input: i) ifAbsent: [ nil ].
    ]

    printOn: stream [
      <category: 'printing'>
      rules printOn: stream
    ]
]

Object subclass: ContextFreeGrammar [
    | rules nonTerminals terminals firsts eps nonEmpty follows |
    ContextFreeGrammar class >> start: s rules: r [
        ^self new start: s rules: r.
    ]

    start: s rules: val [
        | rules_ symbols |
        symbols := Set new add: s; yourself.
        rules_ := Dictionary new.
        rules_ at: s put: Set new.
        "I assume using references to members of rules is safe"
        rules := val copy.

        val collect: [:ass |
            rules_ at: ass key ifAbsentPut:
            [ Set new ].
            (rules_ at: ass key) add: ass value.
            symbols addAll: ass value.
        ].

        nonTerminals := rules_ keys.
        terminals := symbols - nonTerminals.
        self setupFollows.
    ]

    fixedPointIteration: i update: updateBlock estimate: estBlock [
        | prev |
        prev := estBlock value: i.
        [
          updateBlock value: i.
          prev = (estBlock value: i) ifFalse: [ prev := (estBlock value: i) ]; yourself.
        ] whileFalse: [ ].
    ]

    setupEps [
        eps := Set new addAll: ((rules select: [:r | r value = '' ])
                                       collect: [:r | r key ]); yourself.

        self fixedPointIteration: eps update: [:e |
        rules do: [:r |
          ((Set new addAll: (r value); yourself) <= e)
            ifTrue: [ e add: (r key) ].
          ].
        ] estimate: [:e | e size].
    ]

    setupNonEmpty [
        self setupEps.
        nonEmpty := Set new addAll: eps; yourself.
        nonEmpty := nonEmpty + terminals.

        self fixedPointIteration: nonEmpty update: [:n |
        rules do: [:r |
          ((Set new addAll: (r value); yourself) <= n)
            ifTrue: [ n add: (r key) ].
          ].
        ] estimate: [:e | e size].
    ]

    setupFirsts [
        self setupNonEmpty.
        firsts := Dictionary new.
        terminals do: [:t | firsts at: t put: (Set new add: t; yourself) ].
        nonTerminals do: [:nT | firsts at: nT put: (Set new)].

        self fixedPointIteration: firsts
        update: [:first | rules do: [:rule |
          | prev key |
          prev := Set new.
          key := rule key.
          first at: key put: (first at: key) + (self getFirsts: rule value).
        ]]
        estimate: [:first |
          | sum |
          sum := 0.
          (first collect: [:s | s size]) do: [:s | sum := sum + s].
          sum.
        ].
    ]

    getFirsts: aSymbolCollection [
      | prev result |
      prev := Set new.
      result := Set new.

      aSymbolCollection do: [:S | prev <= eps ifTrue: [
          result addAll: (firsts at: S).
        ].
        prev add: S.
      ].

      ^result.
    ]

    setupFollows [
        self setupFirsts.
        follows := Dictionary new.

        terminals + nonTerminals do: [:s | follows at: s put: Set new].

        self fixedPointIteration: follows
        update: [:follow | rules do: [:r |
          | rule prev |
          rule := r value.
          prev := Set new.
          1 to: rule size do: [:i |
             | prevSub term |
             term := rule at: i.
             prev <= eps ifTrue: [
               follow at: term put: (follow at: term) + (follow at: r key).
             ].
             prev add: (rule at: (rule size + 1 - i)).

             prevSub := Set new.
             i+1 to: rule size do: [:j |
               prevSub <= eps ifTrue: [
                 follow at: term put: ((follow at: term) + (firsts at: (rule at: j)))
               ].
               prevSub add: (rule at: j).
            ]
          ].
        ]]
        estimate: [:follow |
          | sum |
          sum := 0.
          (follow collect: [:s | s size]) do: [:s | sum := sum + s].
          sum.
        ].
    ]

    getParserLL1 [
        | parserRules |
        parserRules := Set new.

        rules do: [:rule |
          | first symbol |
          symbol := rule key.
          first := self getFirsts: rule value.

          first do: [:a |  parserRules add:
                           (ParserState symbol: symbol input: a) ->
                           rule value].
          "Check if we can derive righthand side to empty.
           If so, include rules induced by follow sets"
          (Set new addAll: rule value; yourself) <= eps
            ifTrue: [ (follows at: symbol)
                      do: [:b | parserRules add:
                      (ParserState symbol: symbol input: b) ->
                      rule value] ].
        ].
        ^ParserTable empty: eps nonEmpty: nonEmpty rules: parserRules.
    ]

    alphabet [ ^terminals ]
]

Object subclass: ParserLL1 [
    | grammar table stack |
    "Stack as nil means language is empty"
    ParserLL1 class >> start: s rules: r [
        | grammar table stack |
        grammar := ContextFreeGrammar start: s rules: r.
        table := grammar getParserLL1.
        stack := (Stack empty push: s).

        (nil ~= table)
            ifTrue: [ ^self new grammar: grammar table: table
                                stack: stack; yourself ]
            ifFalse: [ ^nil ]
   ]

    grammar: g table: t stack: s [ grammar := g. table := t. stack := s ]
    newParseStack: s [ ^ParserLL1 new grammar: grammar table: table stack: s ]

    accept [ ^stack ~= nil and:
        [stack forAll: [:s | table canDeriveToEmpty: s ]].
    ]
    reject [ ^stack = nil or:
              [(stack forAll: [:s | table canDerive: s ]) not] ]
    predict [ ^grammar alphabet select: [:s | (self react: s) reject not] ]
    react: x [
      | newStack |
      stack = nil | (table symbol: stack top input: x) = nil
        ifTrue: [ ^self newParseStack: nil ].
      newStack := stack duplicate.

      [ (newStack top ~= x) & (newStack empty not) ] whileTrue: [
        newStack pushAll: (table symbol: newStack pop input: x) reverse.
      ].
      newStack empty ifTrue: [ ^self newParseStack: nil ].
      newStack pop.

      ^self newParseStack: newStack.
    ]

    printOn: stream [
      <category: 'printing'>
      table printOn: stream.
      stack printOn: stream.
    ]
]
