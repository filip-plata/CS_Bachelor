#include <stdlib.h>
#include <ctype.h>
#include "evaluatePosition.h"

static int kingValue = 1000000;
static int knightValue = 100;
static int peasantValue = 60;

int getPieceValue(Piece piece) {
    if (piece->letter == 'c' || piece->letter == 'C') {
        return peasantValue;
    }
    if (piece->letter == 'r' || piece->letter == 'R') {
        return knightValue;
    }
    if (piece->letter == 'k' || piece->letter == 'K') {
        return kingValue;
    }
    return -1;
}

int knightAttackValue(Point point, int x, int y, int boardSize) {
    int distX = abs(x - point->x);
    int distY = abs(y - point->y);
    int distance = (distX > distY) ? distX : distY;

    return (boardSize - distance) * (boardSize - distance) * (boardSize -
            distance) * (boardSize - distance);
}

int peasantEvaluate(Piece peasant, int turn) {
    int time = turn - peasant->lastMove;
    if (time > 2) {
        time = 2;
    }

    return time*time*20;
}

int countNeighbours(board Game, int x, int y) {
    int result = 0;
    int n = boardSize(Game);

    for (int i=-2; i<=2; i++) {
        for (int j=-2; j<=2; j++) {
            Point helper = makePoint(x, y);

            if (validatePoint(helper, n) &&
                !isPositionFree(Game, x + i, y + j) &&
                toupper(pieceOnPosition(Game, x + i, y + j)->letter) == 'C') {
                result++;
            }
        }
    }

    return result;
}

int crowdPenalty(int x, int n) {
    if (x < 4) {
        return 0;
    }
    if (x == 4) {
        return n;
    }

    return 10000;
}

int tooManyPeasants(board Game, int player) {
    int peasants = getPeasants(Game);

    if (peasants > 4) {
        return -10000000*peasants;
    }

    return 0;
}

int calculatePiecesValue(board Game, Move move, Point King) {
    int result = 0;

    if (move->movedPiece == NULL) {
        return 0;
    }

    if (move->isPieceRemoved == NULL) {
        result = 0;
    }
    else {
        if (toupper(move->movedPiece->letter) == toupper
        (move->isPieceRemoved->letter)) {
            result = 0;
            if (toupper(move->isPieceRemoved->letter) == 'R') {
                result += knightAttackValue(King,move->xTo, move->yTo, boardSize
                        (Game));
            }
        }
        else {
            if (move->movedPiece != move->isPieceRemoved) {
                result += getPieceValue(move->isPieceRemoved);
            }
            else {
                result -= getPieceValue(move->movedPiece);
            }
        }
    }

    if (move->isPieceCreated != NULL) {
        result += getPieceValue(move->isPieceCreated);
    }

    return result;
}

int calculateTooCrowdedPenalty(board Game, Move move) {
    int result = 0;

    if (move->movedPiece == NULL) {
        return result;
    }

    if (toupper(move->movedPiece->letter) != 'C') {
        return result;
    }

    int neighbours = countNeighbours(Game, move->xTo, move->yTo);
    result -= crowdPenalty(neighbours, boardSize(Game));

    return result;
}

int calculateKnightAttackingKingsValue(board Game, Move move, Point King) {
    int result = 0;

    if (move->movedPiece == NULL ||
        toupper(move->movedPiece->letter) != 'R') {
        return result;
    }

    result += knightAttackValue(King, move->xTo, move->yTo,boardSize(Game));
    result -= knightAttackValue(King, move->xFrom, move->yFrom, boardSize
            (Game));

    return result;
}

int calculatePeasantProductionPotential(board Game, Move move) {
    int result = 0;

    if (move->movedPiece == NULL ||
        toupper(move->movedPiece->letter) != 'C') {
        return result;
    }

    result += peasantEvaluate(move->movedPiece, currentTurn(Game));

    return result;
}

int calculatePerk(board Game, Move move) {
    int result = 0;
    int peas = getPeasants(Game);
    int knig = getKnights(Game);

    if ((move->command == PRODUCE_PEASANT) && (knig >= peas)) {
        result += 10000;
    }

    if ((move->command == PRODUCE_KNIGHT) && (peas > knig)) {
        result += 10000;
    }

    if (move->command == PRODUCE_PEASANT && peas > 5) {
        result -= 1000000;
    }

    return result;
}

int evaluatePosition(board Game, int player, Move move) {
    int result = 0;

    Point myKingPosition = getKingPosition(Game, player);
    Point yourKingPosition = getKingPosition(Game, 3 - player);

    if (myKingPosition == NULL) {
        free(yourKingPosition);
        return -100000000;
    }
    if (yourKingPosition == NULL) {
        free(myKingPosition);
        return 10000000;
    }

    result += calculatePiecesValue(Game, move, myKingPosition);

    result += calculateKnightAttackingKingsValue(Game, move,
    yourKingPosition);

    result += calculatePeasantProductionPotential(Game, move);
    result += tooManyPeasants(Game, player);

    result += calculatePerk(Game, move);

    result += calculateTooCrowdedPenalty(Game, move);

    free(myKingPosition);
    free(yourKingPosition);

    return result;
}



