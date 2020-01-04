#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "move.h"
#include "evaluatePosition.h"
#include "ai.h"

#define INFINITY 2000000000

Move newMove() {
    Move move = malloc(sizeof(struct defMove));

    move->isPieceCreated = NULL;
    move->isPieceRemoved = NULL;
    move->movedPiece = NULL;
    move->xFrom = 0;
    move->xTo = 0;
    move->yTo = 0;
    move->yFrom = 0;
    move->command = DO_NOTHING;
    move->currentPieceTurnAfter = 0;
    move->currentPieceTurnBefore = 0;

    return move;
}

void doMove(board Game, Move move) {

    if (move->command == DO_NOTHING)
        return;

    if (move->isPieceCreated != NULL) {
        putPiece(Game, move->xTo, move->yTo, move->isPieceCreated);
    }
    else {
        removePiece(Game, move->xFrom, move->yFrom);
        if (move->isPieceRemoved == NULL) {
            putPiece(Game, move->xTo, move->yTo, move->movedPiece);
        }
        else {
            if (move->movedPiece != move->isPieceRemoved) {
                removePiece(Game, move->xTo, move->yTo);
                if (toupper(move->movedPiece->letter) !=
                    toupper(move->isPieceRemoved->letter)) {
                    putPiece(Game, move->xTo, move->yTo, move->movedPiece);
                }
            }
        }
    }

    move->movedPiece->lastMove = move->currentPieceTurnAfter;
}

void undoMove(board Game, Move move) {

    if (move->command == DO_NOTHING)
        return;

    if (move->isPieceCreated != NULL) {
        removePiece(Game, move->xTo, move->yTo);
    }
    else {
        putPiece(Game, move->xFrom, move->yFrom, move->movedPiece);
        if (move->isPieceRemoved != NULL &&
            move->isPieceRemoved != move->movedPiece) {
            removePiece(Game, move->xTo, move->yTo);
            putPiece(Game, move->xTo, move->yTo, move->isPieceRemoved);
        }
        if (move->isPieceRemoved == NULL) {
            removePiece(Game, move->xTo, move->yTo);
        }
    }

    move->movedPiece->lastMove = move->currentPieceTurnBefore;
}

Move endTurnMove() {
    Move result = newMove();
    result->command = END_TURN;
    return result;
}

void printMoveCommand(Move move) {
    if (move->command == END_TURN) {
        printf("END_TURN\n");
    }
    else {
        if (move->command == PRODUCE_KNIGHT) {
            printf("PRODUCE_KNIGHT");
        }
        if (move->command == PRODUCE_PEASANT) {
            printf("PRODUCE_PEASANT");
        }
        if (move->command == MOVE) {
            printf("MOVE");
        }
        printf(" %d %d %d %d\n", move->xFrom, move->yFrom, move->xTo, move->yTo);
    }
}

Move moveBuilder(Point position, Piece piece, board GameState) {
    Move result = malloc(sizeof(struct defMove));

    result->xFrom = position->x;
    result->yFrom = position->y;
    result->movedPiece = piece;
    result->currentPieceTurnBefore = piece->lastMove;
    result->currentPieceTurnAfter = currentTurn(GameState);
    result->command = DO_NOTHING;
    result->isPieceCreated = NULL;
    result->isPieceRemoved = NULL;

    return result;
}

Move constructMoveCommand(int x, int y, Move result, board Game) {

    Piece piece = pieceOnPosition(Game, result->xFrom, result->yFrom);
    result->xTo = x;
    result->yTo = y;
    result->isPieceCreated = NULL;

    if (isPositionFree(Game, x, y)) {
        result->isPieceRemoved = NULL;
    }
    else {
        Piece piece1 = pieceOnPosition(Game, x, y);
        Piece winner = tournament(piece, piece1);
        result->isPieceRemoved = (winner != piece) ? piece : piece1;
        if (winner == NULL) {
            result->isPieceRemoved = piece1;
        }
    }

    return result;
}

void copyMoveData(Move move1, Move move2) {
    if (move2->isPieceCreated != NULL) {
        move1->isPieceCreated = pieceCopy(move2->isPieceCreated);
    }
    else {
        move1->isPieceCreated = NULL;
    }
    move1->isPieceRemoved = move2->isPieceRemoved;

    move1->movedPiece = move2->movedPiece;

    move1->command = move2->command;
    move1->xFrom = move2->xFrom;
    move1->xTo = move2->xTo;
    move1->yFrom = move2->yFrom;
    move1->yTo = move2->yTo;
    move1->currentPieceTurnAfter = move2->currentPieceTurnAfter;
    move1->currentPieceTurnBefore = move2->currentPieceTurnBefore;
}

void cleanMoveData(Move move) {
    free(move->isPieceCreated);
    free(move);
}

int getMovesCommands(board Game, Piece piece, Move *movesTable, Point point) {

    int n = 1;
    Move result = moveBuilder(point, piece, Game);
    int board_Size = boardSize(Game);
    int x = point->x;
    int y = point->y;

    result->command = MOVE;

    for (int j=-1; j<=1; j++) {
        for (int i=-1; i<=1; i++) {
            if ( (j != 0 ||
                  i != 0) &&
                 validatePoint(makePoint(x + i, y + j), board_Size)) {

                Piece piece1 = pieceOnPosition(Game, x + i, y + j);

                if (piece1 != NULL &&
                    piece1->owner == piece->owner) {
                    continue;
                }

                constructMoveCommand(x+i, y+j, result, Game);

                movesTable[n] = newMove();
                copyMoveData(movesTable[n], result);
                n++;
            }
        }
    }

    free(result);

    return n;
}

char matchingKnightLetter(char letter) {
    if (letter == 'c') {
        return 'r';
    }
    else {
        return 'R';
    }
}

int getPossibleMoves(board Game, Piece piece, Move *movesTable, Point point) {
    int n = 0;

    movesTable[n] = newMove();
    n++;

    if (piece->lastMove == currentTurn(Game)) {
        return n;
    }

    n = getMovesCommands(Game, piece, movesTable, point);

    Move result = moveBuilder(point, piece, Game);
    int board_Size = boardSize(Game);
    int x = point->x;
    int y = point->y;


    if (toupper(piece->letter) == 'C' &&
            currentTurn(Game) - piece->lastMove >=3 ) {
        for (int j=-1; j<=1; j++) {
            for (int i=-1; i<=1; i++) {
                if ( (j != 0 ||
                     i != 0) &&
                     validatePoint(makePoint(x + i, y + j), board_Size) &&
                     isPositionFree(Game, x + i, y + j)) {
                    result->xTo = x + i;
                    result->yTo = y + j;
                    result->isPieceRemoved= NULL;


                    char letter = piece->letter;
                    Piece pieceCreated = createNewPiece(piece->owner, letter,
                                                        currentTurn(Game));
                    result->isPieceCreated = pieceCreated;
                    result->command = PRODUCE_PEASANT;
                    movesTable[n] = newMove();
                    copyMoveData(movesTable[n], result);
                    n++;

                    free(result->isPieceCreated);

                    letter = matchingKnightLetter(letter);

                    pieceCreated = createNewPiece(piece->owner, letter,
                                                  currentTurn(Game) - 1);
                    result->isPieceCreated = pieceCreated;
                    result->command = PRODUCE_KNIGHT;
                    movesTable[n] = newMove();
                    copyMoveData(movesTable[n], result);
                    n++;

                    free(result->isPieceCreated);
                }
            }
        }
    }

    free(result);

    return n;
}


Move getBestMove(board Game, Piece piece, Point point) {
    Move possibleMoves[26];
    int possibleMovesNumber = getPossibleMoves(Game,
                                               piece,
                                               possibleMoves,
                                               point);
    Move result = NULL;
    int bestValue = -INFINITY;

    for (int i = 0; i < possibleMovesNumber; i++) {
        doMove(Game, possibleMoves[i]);

        int moveValue = evaluatePosition(Game, aiPlayerNumber,
                                         possibleMoves[i]);

        if (moveValue > bestValue) {
            result = possibleMoves[i];
            bestValue = moveValue;
        }
        undoMove(Game, possibleMoves[i]);

    }

    for (int i=0; i<possibleMovesNumber; i++) {
        if (result != possibleMoves[i]) {
            cleanMoveData(possibleMoves[i]);
        }
    }

    return result;
}


