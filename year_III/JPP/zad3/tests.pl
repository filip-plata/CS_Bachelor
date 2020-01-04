%%%%%%%% README %%%%%%%%
% Place next to yout soultion and replace consult line
% below with apropirate name
:- begin_tests(lists).
:- use_module(library(lists)).
% replace with your own module name
:- consult(fp371335).

%%%%%%% test jestWyborem start

test(jestWyborem_NoE) :-
  jestWyborem([[v0, a, v1, v2], [v1, a, v3], [v2, a, v0], [v3, a]],
              [[v0, a, v1, v2], [v1, a, v3], [v2, a, v0], [v3, a]]).

test(jestWyborem_Small) :-
  jestWyborem([[v0, a, v1, v4], [v4, e, v0], [v2, e, v0, v1, v3], [v3, e, v0], [v1, a, v4]],
              [[v0, a, v1, v4], [v4, e, v0], [v2, e, v3], [v3, e, v0], [v1, a, v4]]).

test(jestWyborem_Empty) :-
  jestWyborem([], []).

%%%%%%% test jestWyborem end

%%%%%%% test jestDFS start

test(jestDFS_Clique) :-
        jestDFS([[v0, a, v1, v2, v3], [v1, a, v0, v2, v3],
                 [v2, a, v0, v1, v3], [v3, a, v0, v1, v2]],
                 [v0, v3, v2, v1]).

test(jestDFS_Cycle) :-
  jestDFS([[v0, a, v1, v3], [v1, a, v2], [v2, e, v0], [v3, a]],
          [v0, v3, v1, v2]).

test(jestDFS_Empty) :-
  jestDFS([], []).

test(jestDFS_OneNode) :-
  jestDFS([[v0, a]], [v0]).

test(jestDFS_TwoNodes) :-
  jestDFS([[v1, a, v0], [v0, e, v1]], [v1, v0]).

test(jestDFS_Large) :-
  jestDFS([[v0, e, v3, v4], [v3, a, v5, v6], [v4, a, v6], [v1, a, v2],
           [v2, a, v5, v0], [v5, a], [v6, e, v1]],
           [v0, v4, v6, v1, v2, v5, v3]).

%%%%%%% test jestDFS end

%%%%%%% test jestADFS1 start

%%% idea of testing jestADFS is a joke. Code it right

%%%%%%% test jestADFS1 end

%%%%%%% test jestADFS1 start

test(jestADFS1_Empty) :-
  jestADFS1([], []).

test(jestADFS1_Simple) :-
  jestADFS1([[v0, e, v1, v2], [v1, a], [v2, a]], [v0, v1]).

test(jestADFS1_Cycle) :-
  jestADFS1([[v0, e, v1, v2], [v1, e, v0, v2], [v2, e, v1, v2]],
            [v0, v2, v1]).

test(jestADFS1_TaskExample) :-
  jestADFS1([[v0, a, v1, v3], [v1, e, v2], [v2, a], [v3, a]], [v0, v3, v1, v2]).

test(jestADFS1_Clique) :-
  jestADFS1([[v0, e, v1, v2, v3], [v1, e, v0, v2, v3],
            [v2, e, v0, v1, v3], [v3, e, v0, v1, v2]],
            [v0, v3, v2, v1]).

test(jestADFS1_Large) :-
  jestADFS1([[v0, a, v4, v6], [v4, e, v3, v0], [v3, a, v2, v1, v6],
             [v1, a], [v2, e, v6, v5], [v5, e, v6, v0], [v6, a]],
             [v0, v6, v4]).

%%%%%%% test jestADFS1 end

:- end_tests(lists).
