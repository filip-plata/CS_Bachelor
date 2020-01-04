% Filip Plata, fp371335

% Not implemented:
% - passing multiple unspecified arguments to predicates

%%%%%%%%%%%%%%%% Utils start

% Check if node is of type A
% Assumes correct graph input
is_A([[V, T | _] | _], V) :- T = a.
is_A([_ | G], V) :- is_A(G, V).

% Check if node is of type E
% Assumes correct graph input
is_E([[V, T | _] | _], V) :- T = e.
is_E([_ | G], V) :- is_E(G, V).

% Get edges for node
edgesAE([[U, _ | E] | _], U, E).
edgesAE([_ | G1], U, E) :- edgesAE(G1, U, E).

% Aim of this function is to prevent m*m
% time complexity in DFS

% If Visited is already a member of L, then the result is L
ensure(Visited, L, L) :-
    member(Visited, L), !.

% If not, the result is Visited appended to L
ensure(Visited, L, [Visited | L]).

% SICSTUS is so poor even this has to be implemented
subset([], _) :- !.
subset([E|R], Set) :-
    memberchk(E, Set),
subset(R, Set).

intersection([], _, []) :- !.
intersection([X|T], L, Intersect) :-
    memberchk(X, L),
    !,
    Intersect = [X|R],
    intersection(T, L, R).
intersection([_|T], L, R) :-
intersection(T, L, R).

%%%%%%%%%%%%%%%% Utils end

%%%%%%%%%%%%%%%% jestWyborem start

% On A node we simply copy edges
jestWyborem([[V, T | Vs] | D], [[V, T | Vs] | G1]) :- T = a, jestWyborem(D, G1).
% On E node choose one edge
jestWyborem([[V, T | Vs] | D], [[V, T, V1] | G1]) :- T = e, member(V1, Vs), jestWyborem(D, G1).
jestWyborem([], []).

%%%%%%%%%%%%%%%% jestWyborem end

%%%%%%%%%%%%%%%% jestDFS start

% First argument is a graph
% Second is a stack of visited nodes
% Third argument keeps list of already visited nodes
% Fourth is result path
jestDFS(G, [U | Stack], Visited, [W | Ls]) :-
  edgesAE(G, U, Eall),
  member(W, Eall),
  \+member(W, Visited),
  ensure(U, Visited, Visited1),
  jestDFS(G, [W, U | Stack], Visited1, Ls).
jestDFS(G, [U | Stack], Visited, Ls) :-
  edgesAE(G, U, Eall),
  subset(Eall, Visited),
  ensure(U, Visited, Visited1),
  jestDFS(G, Stack, Visited1, Ls).
jestDFS(_, [], _, []).

% On stack there is first node and nothing is yet visited
jestDFS([[V | Vs] | G1], [V | L]) :- jestDFS([[V | Vs] | G1], [V], [], L).
jestDFS([], []).

%%%%%%%%%%%%%%%% jestDFS end

%%%%%%%%%%%%%%%% jestADFS start

jestADFS(G, L) :- jestWyborem(G, G1), jestDFS(G1, L).

%%%%%%%%%%%%%%%% jestADFS end

%%%%%%%%%%%%%%%% jestADFS1 start

% Conceptually similar to jestDFS - A nodes behave
% exactly the same. E nodes have differences
% noted in comments.
jestADFS1(G, [U | Stack], Visited, [W | Ls]) :-
  is_A(G, U),
  edgesAE(G, U, Eall),
  member(W, Eall),
  \+member(W, Visited),
  ensure(U, Visited, Visited1),
  jestADFS1(G, [W, U | Stack], Visited1, Ls).
jestADFS1(G, [U | Stack], Visited, [W | Ls]) :-
  is_E(G, U),
  % an E type node can be visited only once
  \+member(U, Visited),
  edgesAE(G, U, Eall),
  member(W, Eall),
  \+member(W, Visited),
  ensure(U, Visited, Visited1),
  jestADFS1(G, [W, U | Stack], Visited1, Ls).
jestADFS1(G, [U | Stack], Visited, Ls) :-
  is_A(G, U),
  edgesAE(G, U, Eall),
  subset(Eall, Visited),
  ensure(U, Visited, Visited1),
  jestADFS1(G, Stack, Visited1, Ls).
jestADFS1(G, [U | Stack], Visited, Ls) :-
  is_E(G, U),
  edgesAE(G, U, Eall),
   % To pop from stack we have to be able to
   % choose edge to already visited node
  intersection(Visited, Eall, [_ | _]),
  ensure(U, Visited, Visited1),
  jestADFS1(G, Stack, Visited1, Ls).
jestADFS1(_, [], _, []).

jestADFS1([[V | Vs] | G1], [V | L]) :- jestADFS1([[V | Vs] | G1], [V], [], L).
jestADFS1([], []).

%%%%%%%%%%%%%%%% jestADFS1 end
