#ifndef MIDDLE_AGES_PIECE_H
#define MIDDLE_AGES_PIECE_H

/**
 * @file
 * File contains definition of piece used in game.
 * All other necessary functions are also included.
 */

#include <stdbool.h>

/**
 * @struct def_piece
 * @brief Represents a piece in program.
 */
struct def_piece {
    int owner; /**< Owner of the piece. Possible values are 1 and 2.*/
    char letter; /**< Describes type of a piece.*/
    int lastMove; /**< Number of turn in which the piece has
 *                     performed action last time.*/
};

/**
 * @typedef Piece
 * @brief Pointer handle for def_piece.
 */
typedef struct def_piece *Piece;

/**
 * Constructor of a piece.
 * @param[in] owner
 * @param[in] letter
 * @param[in] lastMove
 */
Piece createNewPiece(int owner, char letter, int lastMove);

/**
 * Returns result of two pieces fighting.
 * Returns NULL in case of a draw.
 * @param[in] piece1 First piece.
 * @param[in] piece2 Second piece.
 */
Piece tournament(Piece piece1,
                 Piece piece2);

/**
 * Destructor of piece.
 * Used in map module.
 * @param[in] piece Piece to be freed from memory.
 */
void destroyPiece(struct def_piece *piece);

/**
 * Tests whether pieces are equal. They are considered such
 * iff all their fields are equal.
 */
bool equalPiece(Piece piece1, Piece piece2);

/**
 * Returns a copy of a piece. All fields
 * are equal.
 * @param[in] piece Piece to copy.
 */
Piece pieceCopy(Piece piece);

#endif //MIDDLE_AGES_PIECE_H
