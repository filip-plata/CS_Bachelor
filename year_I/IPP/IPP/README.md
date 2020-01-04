AI for a game, implemented over the course of the semester.

Repositories were created automatically for the purpose
 of grading, thus two contributtors appear.

Game is probably well known - there are two players, each one of them has
at the begginig a king, a peasant and two knights. Every three turns the peasant.
can produce a knight or a peasant(provided the peasant stood still for those 3 turns).

Pieces can kill opponent's pieces, according to their relative strength:
knight > king > peasant. Two pieces of the same type kill each other.

Goal is to kill king before game ends. Game consist of some number of turns,
in every turn every piece can perform one move, eg. move to adjacent square or
produce another unit(only peasant can do that!).

It was used as a task for Individual Programming Project on University of Warsaw,
last part of it being implementing simple AI, which I took further. Because of it,
there is possibility in project there are some(up to 10) lines not written by me
(in doxygen documentation).

Game can be run on with GUI(game.sh), but also in text line mode.
