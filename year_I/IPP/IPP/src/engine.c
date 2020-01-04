#include <stdlib.h>
#include <ctype.h>
#include "board.h"
#include "ai.h"


static board GameBoard;
static char king[3];
static char knight[3];
static char peasant[3];

void startGame() {

    GameBoard = newGameBoard();
    king[1] = 'K';
    king[2] = 'k';
    knight[1] = 'R';
    knight[2] = 'r';
    peasant[1] = 'C';
    peasant[2] = 'c';
}

void definitlyKillPiece(Piece piece) {
    if (toupper(piece->letter) == 'K') {
        killKing(GameBoard, piece->owner);
    }
    if (toupper(piece->letter) == 'C') {
        incrementPeasants(GameBoard, -1);
    }
    if (toupper(piece->letter) == 'R') {
        incrementKnights(GameBoard, -1);
    }
    destroyPiece(piece);
}

void endGame() {
    deleteGameBoard(GameBoard);
}

void initializePlayerPieces(int player, int x, int y) {
    Piece PieceK = createNewPiece(player, king[player], 0);
    Piece PieceR1 = createNewPiece(player, knight[player], 0);
    Piece PieceR2 = createNewPiece(player, knight[player], 0);
    Piece PieceC = createNewPiece(player, peasant[player], 0);

    putPiece(GameBoard, x, y, PieceK);
    putPiece(GameBoard, x + 2, y, PieceR1);
    putPiece(GameBoard, x + 3, y, PieceR2);
    putPiece(GameBoard, x + 1, y, PieceC);
}

int init(int n, int k, int p, int x1, int y1, int x2, int y2) {
    setGameValues(GameBoard, n, k, x1, y1, x2, y2);
    aiPlayerNumber = p;

    int x = 0;
    int y = 0;

    if (p == 1) {
        x = x1;
        y = y1;
    }
    if (p == 2) {
        x = x2;
        y = y2;
    }

    initializePlayerPieces(p, x, y);

    if (p == 2) {
        x = x1;
        y = y1;
    }
    if (p == 1) {
        x = x2;
        y = y2;
    }

    initializePlayerPieces(3 - p, x, y);

    return 0;
}

int move(int x1, int y1, int x2, int y2) {
    Piece piece1 = pieceOnPosition(GameBoard, x1, y1);
    Piece piece2 = pieceOnPosition(GameBoard, x2, y2);
    int player = currentPlayerNumber(GameBoard);
    int turn = currentTurn(GameBoard);


    if (piece1 == NULL) {
        return 1;
    }

    if (piece1->lastMove >= turn) {
        return 1;
    }

    if (piece1 == piece2) {
        return 0;
    }

    if (piece1->owner != player) {
        return 1;
    }

    if (piece2 == NULL) {
        removePiece(GameBoard, x1, y1);
        putPiece(GameBoard, x2, y2, piece1);
        piece1->lastMove = turn;

        return 0;
    }

    if (piece2->owner == piece1->owner) {
        return 1;
    }

    Piece winningPiece = tournament(piece1, piece2);
    removePiece(GameBoard, x1, y1);
    removePiece(GameBoard, x2, y2);

    if (winningPiece != NULL) {
        putPiece(GameBoard, x2, y2, winningPiece);

        if (winningPiece == piece1) {
            winningPiece->lastMove = turn;
            definitlyKillPiece(piece2);
        }
        else {
            definitlyKillPiece(piece1);
        }
    }
    else {
        definitlyKillPiece(piece1);
        definitlyKillPiece(piece2);
    }

    return 0;
}

bool is_valid_peasant(Piece piece, int player) {
    int owner = piece->owner;

    if (owner != player || piece->letter != peasant[owner]) {
        return false;
    }

    if (currentTurn(GameBoard) - piece->lastMove <= 2) {
        return false;
    }

    return true;
}

int produce_piece(char letter, int x1, int y1, int x2, int y2) {
    int player = currentPlayerNumber(GameBoard);
    int current_Turn = currentTurn(GameBoard);
    Piece peasant = pieceOnPosition(GameBoard, x1, y1);

    if (peasant == NULL) {
        return 1;
    }

    if (!is_valid_peasant(peasant, player)) {
        return 1;
    }

    Piece new_piece = createNewPiece(player, letter, current_Turn - 1);
    removePiece(GameBoard, x1, y1);
    putPiece(GameBoard, x1, y1, new_piece);

    if (move(x1, y1, x2, y2) == 0) {
        putPiece(GameBoard, x1, y1, peasant);
        peasant->lastMove = current_Turn;
        if (new_piece == pieceOnPosition(GameBoard, x2, y2)) {
            new_piece->lastMove = current_Turn - 1;
        }
        return 0;
    }
    else {
        destroyPiece(new_piece);
        return 1;
    }
}

int produceKnight(int x1, int y1, int x2, int y2) {
    int player = currentPlayerNumber(GameBoard);

    incrementKnights(GameBoard, 1);

    return produce_piece(knight[player], x1, y1, x2, y2);
}

int producePeasant(int x1, int y1, int x2, int y2) {
    int player = currentPlayerNumber(GameBoard);
    incrementPeasants(GameBoard, 1);
    return produce_piece(peasant[player], x1, y1, x2, y2);
}

int endTurn() {
    if (currentPlayerNumber(GameBoard) == 2) {
        incrementTurn(GameBoard);
    }
    updateCurrentPlayerNumber(GameBoard);

    return 0;
}

bool gameOver() {
    if (getKing(GameBoard, 1) == 0 ||
        getKing(GameBoard, 2) == 0 ||
            currentTurn(GameBoard) > turnsLimit(GameBoard)) {
        return true;
    }
    else {
        return false;
    }
}

int gameResult() {
    int king1 = getKing(GameBoard, aiPlayerNumber);
    int king2 = getKing(GameBoard, 3 - aiPlayerNumber);

    if ((king1 && king2) || (!king1 && !king2)) {
        return 1;
    }

    if (king1 == 0) {
        return 2;
    }

    return 1;
}

void performMoves() {
    static Move aiCommands[402];
    //board gameCopy = boardCopy(GameBoard);
    board gameCopy = newGameBoard();

    int n = fetchAiCommands(gameCopy, aiCommands);

    for (int i = 0; i < n && !gameOver(); i++) {
        if (aiCommands[i]->command == DO_NOTHING) {
            continue;
        }

        printMoveCommand(aiCommands[i]);
        if (aiCommands[i]->command == END_TURN) {
            endTurn();
        }
        if (aiCommands[i]->command == MOVE) {
            move(aiCommands[i]->xFrom,
                 aiCommands[i]->yFrom,
                 aiCommands[i]->xTo,
                 aiCommands[i]->yTo);
        }
        if (aiCommands[i]->command == PRODUCE_PEASANT) {
            producePeasant(aiCommands[i]->xFrom,
                           aiCommands[i]->yFrom,
                           aiCommands[i]->xTo,
                           aiCommands[i]->yTo);
        }
        if (aiCommands[i]->command == PRODUCE_KNIGHT) {
            produceKnight(aiCommands[i]->xFrom,
                          aiCommands[i]->yFrom,
                          aiCommands[i]->xTo,
                          aiCommands[i]->yTo);
        }
    }

    for (int i=0; i<n; i++) {
        if (aiCommands[i]->isPieceRemoved != NULL &&
                toupper(aiCommands[i]->movedPiece->letter) ==
                toupper(aiCommands[i]->isPieceRemoved->letter)) {
            free(aiCommands[i]->movedPiece);
        }
        free(aiCommands[i]->isPieceRemoved);
        free(aiCommands[i]);
    }

    deleteGameBoard(gameCopy);
}
