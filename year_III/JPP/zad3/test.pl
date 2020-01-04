g0([[v0, a, v1], [v1, a, v0]]).
?- g0(Y), \+jestDFS(Y, [v1, v0]).
?- g0(Y), jestDFS(Y, [v0, v1]).
g1([[v0, a, v1, v3], [v1, a, v2, v3], [v2, a, v3], [v3, a, v2]]).
?- g1(Y), jestDFS(Y, [v0, v1, v2, v3]).
?- g1(Y), jestDFS(Y, [v0, v3, v2, v1]).
?- g1(Y), \+jestDFS(Y, [v0, v3, v1, v2]).
g2([[v0, a, v1, v2], [v1, a, v0, v2], [v2, a, v3], [v3, a]]).
?- g2(Y), jestDFS(Y, [v0, v1, v2, v3]).
?- g2(Y), jestDFS(Y, [v0, v2, v3, v1]).
?- g2(Y), \+jestDFS(Y, [v0, v2, v1, v3]).
?- g2(Y), \+jestDFS(Y, [v1, v0, v2, v3]).

?- g2(Y), jestADFS(Y, [v0, v1, v2, v3]).
?- g2(Y), jestADFS(Y, [v0, v2, v3, v1]).
?- g2(Y), \+jestADFS(Y, [v0, v2, v1, v3]).
?- g2(Y), \+jestADFS(Y, [v1, v0, v2, v3]).

clique4([[v0, a, v1, v2, v3, v4], [v1, a, v0, v2, v3, v4], [v2, a, v0, v1, v3, v4], [v3, a, v0, v1, v2, v4], [v4, a, v0, v1, v2, v3]]).
?- clique4(Y), jestDFS(Y, [v0, v1, v2, v3, v4]).
?- clique4(Y), jestDFS(Y, [v0, v3, v2, v1, v4]).
?- clique4(Y), \+jestDFS(Y, [v0, v3, v2, v1]).
?- clique4(Y), setof(X, jestDFS(Y, X), Res), length(Res, 24).
?- jestDFS([[v0, v1, v2, v3], [v1, v0, v2, v3], [v2, v0, v1, v3], [v3, v0, v1, v2]], X).

aeg1([[v0, e, v1, v2], [v1, a, v3], [v2, a, v3], [v3, a]]).
aeg1t([[v1, a, v3], [v2, a, v3], [v3, a]]).
?- aeg1(Y), jestADFS1(Y, [v0, v1, v3]).
?- aeg1(Y), \+jestADFS1(Y, [v0, v1, v2, v3]).
?- aeg1(Y), \+jestADFS1(Y, [v0, v1, v3, v2]).
?- aeg1(Y), jestADFS(Y, [v0, v1, v3]).
?- aeg1(Y), \+jestADFS(Y, [v0, v1, v2, v3]).
?- aeg1(Y), \+jestADFS(Y, [v0, v1, v3, v2]).

?- aeg1(Y), aeg1t(Z), jestWyborem(Y, [[v0, e, v1] | Z]).
?- aeg1(Y), aeg1t(Z), jestWyborem(Y, [[v0, e, v2] | Z]).
?- aeg1(Y), aeg1t(Z), \+jestWyborem(Y, [[v0, e] | Z]).
?- aeg1(Y), aeg1t(Z), \+jestWyborem(Y, [[v0, e, v1, v2] | Z]).
?- aeg1(Y), aeg1t(Z), \+jestWyborem(Y, [[v0, e, v1], [v1, a], [v2, a], [v3 ,a]]).
?- aeg1(Y), aeg1t(Z), \+jestWyborem(Y, [[v0, e], [v1, a, v3], [v2, a, v3], [v3, a]]).

?- jestWyborem(X, [[v0, a, v1, v2]]).
?- jestWyborem([[v0, a, v1, v2]], [[v0, a, v1, v2]]).
?- jestWyborem(X, [[v0, e, v1]]).
?- jestWyborem([[v0, e, v1, v2]], X).

aeg2([[v0, e, v1, v2], [v1, a, v3], [v2, a, v3], [v3, a, v2]]).
?- aeg2(Y), jestADFS1(Y, [v0, v2, v3]).
?- aeg2(Y), \+jestADFS1(Y, [v0, v1, v2, v3]).
?- aeg2(Y), jestADFS1(Y, [v0, v1, v3, v2]).
?- aeg2(Y), jestADFS(Y, [v0, v2, v3]).
?- aeg2(Y), \+jestADFS(Y, [v0, v1, v2, v3]).
?- aeg2(Y), jestADFS(Y, [v0, v1, v3, v2]).
