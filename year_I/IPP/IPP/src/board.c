#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>
#include <stdio.h>
#include "map.h"

struct defBoard {
    Map positionMap;
    long size;
    int current_player;
    long current_turn;
    int turns_limit;
    int isKingAlive1;
    int isKingAlive2;
    int myKnights;
    int myPeasants;
    Point KingPosition[3];
    int King_1_x;
    int King_1_y;
    int King_2_x;
    int King_2_y;
};



typedef struct defBoard *board;

void incrementPeasants(board x, int n) {
    x->myPeasants += n;
}

void incrementKnights(board x, int n) {
    x->myKnights += n;
}

int getPeasants(board x) {
    return x->myPeasants;
}

int getKnights(board x) {
    return x->myKnights;
}

board newGameBoard() {
    board result = (board) malloc(sizeof(struct defBoard));
    result->isKingAlive1 = 1;
    result->isKingAlive2 = 1;
    result->current_player = 1;
    result->current_turn = 1;
    result->positionMap = newMap(comparePoints, destroyPiece);
    result->myPeasants = 0;
    result->myKnights = 0;
    result->KingPosition[1] = makePoint(0, 0);
    result->KingPosition[2] = makePoint(0, 0);

    return result;
}

Piece pieceOnPosition(board Board, int x, int y) {
    struct defPoint z;
    z.x = x;
    z.y = y;
    return (Piece) getValue(Board->positionMap, &z);
}

void removePiece(board Board, int x, int y) {
    struct defPoint z;
    z.x = x;
    z.y = y;
    removeValue(Board->positionMap, &z);
}

void putPiece(board Board, int x, int y, Piece piece) {
    Point z = (Point) malloc(sizeof(struct defPoint));
    z->x = x;
    z->y = y;
    insertValue(Board->positionMap, z, piece);
}

bool isPositionFree(board Board, int x, int y) {
    struct defPoint z;
    z.x = x;
    z.y = y;
    if (getValue(Board->positionMap, &z) == NULL) {
        return true;
    }
    else {
        return false;
    }
}

long boardSize(board x) {
    return x->size;
}

long turnsLimit(board x) {
    return x->turns_limit;
}

void killKing(board x, int player) {
    if (player == 1) {
        x->isKingAlive1 = 0;
    }
    if (player == 2) {
        x->isKingAlive2 = 0;
    }
}

int getKing(board x, int player) {
    if (player == 1) {
        return x->isKingAlive1;
    }
    if (player == 2) {
        return x->isKingAlive2;
    }

    //Should not happen
    return -5;
}

int currentPlayerNumber(board x) {
    return x->current_player;
}

long currentTurn(board x) {
    return x->current_turn;
}

void incrementTurn(board x) {
    x->current_turn++;
}

void updateCurrentPlayerNumber(board x) {
    x->current_player = (3 - x->current_player);
}

void setGameValues(board x, int size, int turns_limit, int x1, int y1, int
x2, int y2) {
    x->size = size;
    x->turns_limit = turns_limit;
    x->myKnights = 2;
    x->myPeasants = 1;
    x->KingPosition[1]->x = x1;
    x->KingPosition[1]->y = y1;
    x->KingPosition[2]->x = x2;
    x->KingPosition[2]->y = y2;
    x->King_1_x = x1;
    x->King_1_y = y1;
    x->King_2_x = x2;
    x->King_2_y = y2;
}

void deleteGameBoard(board Board) {
    destroyMap(Board->positionMap);
    free(Board->KingPosition[1]);
    free(Board->KingPosition[2]);
    free(Board);
}

board boardCopy(board Board) {
    board result = newGameBoard();
    result->size = boardSize(Board);
    result->current_turn = Board->current_turn;

    for (int x=1; x<=result->size; x++) {
        for (int y=1; y<=result->size; y++) {
            Piece currentPiece = pieceOnPosition(Board, x, y);
            if (currentPiece != NULL) {
                putPiece(result, x, y, pieceCopy(currentPiece));
            }
        }
    }

    result->myKnights = Board->myKnights;
    result->myPeasants = Board->myPeasants;

    result->King_1_x = Board->King_1_x;
    result->King_1_y = Board->King_1_y;
    result->King_2_x = Board->King_2_x;
    result->King_2_y = Board->King_2_y;

    return result;
}

Point getKingPosition(board Game, int player) {
    Point result = NULL;

    if (player == 1) {
        result = makePoint(Game->King_1_x, Game->King_1_y);
    }
    else {
        result = makePoint(Game->King_2_x, Game->King_2_y);
    }

    return result;
}

