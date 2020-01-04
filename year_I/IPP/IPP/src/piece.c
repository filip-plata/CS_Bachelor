#include <stdlib.h>
#include <ctype.h>
#include "piece.h"


Piece createNewPiece(int owner, char letter, int whenAvailable) {
    Piece piece = malloc(sizeof(struct def_piece));

    piece->owner = owner;
    piece->letter = letter;
    piece->lastMove = whenAvailable;

    return piece;
}


Piece tournament(Piece piece1, Piece piece2) {
    if (toupper(piece1->letter) == toupper(piece2->letter)) {
        return NULL;
    }

    return (toupper(piece1->letter) > toupper(piece2->letter))
           ? piece1 : piece2;
}


void destroyPiece(Piece piece) {
    free(piece);
}

Piece pieceCopy(Piece piece) {
    return createNewPiece(piece->owner, piece->letter, piece->lastMove);
}

bool equalPiece(Piece piece1, Piece piece2) {
    if (piece1 == NULL && piece2 ==NULL)
        return true;
    if (piece1 == NULL || piece2 == NULL)
        return false;

    if (piece1->letter == piece2->letter &&
            piece1->owner == piece2->owner &&
            piece1->lastMove == piece2->lastMove) {
        return true;
    }

    return false;
}

