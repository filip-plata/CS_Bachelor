#include <stdlib.h>
#include "ai.h"

int fetchAiCommands(board Game, Move* commands) {
    int k=0;
    /*int n = boardSize(Game);
    int nowTurn = currentTurn(Game);

    for (int x=1; x<=n; x++) {
        for (int y=1; y<=n; y++) {
            Piece currentPiece = pieceOnPosition(Game, x, y);

            Point point = makePoint(x, y);

            if (currentPiece != NULL &&
                currentPiece->owner == aiPlayerNumber &&
                currentPiece->lastMove < nowTurn) {
                commands[k] = getBestMove(Game,
                                          currentPiece,
                                          point);
                k++;
                doMove(Game, commands[k-1]);
            }
            free(point);
        }
    }*/
    commands[k++] = endTurnMove();

    return k;
}