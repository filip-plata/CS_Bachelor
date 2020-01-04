
#ifndef MIDDLE_AGES_MOVE_H
#define MIDDLE_AGES_MOVE_H

#include "piece.h"
#include "board.h"
#include "point.h"

enum COMMAND_TYPE {
    MOVE,
    PRODUCE_PEASANT,
    PRODUCE_KNIGHT,
    END_TURN,
    DO_NOTHING,
};

enum COMMAND_TYPE;

typedef struct defMove *Move;

/**
 * @struct defMove
 * @brief Contains all necessary move data.
 */
typedef struct defMove {
    enum COMMAND_TYPE command; /**< Either MOVE or PRODUCE_STH or DO_NOTHING */

    Piece movedPiece; /**< Pointer to the moved piece. */
    int xFrom; /**< Column number before move. */
    int yFrom; /**< Row number before move. */
    int xTo; /**< Column number after move. */
    int yTo; /**< Row number after move. */
    int currentPieceTurnBefore; /**< Turn number before move */
    int currentPieceTurnAfter;/**< Turn number after move */

    Piece isPieceCreated; /**< Pointer to created piece, null otherwise. */
    Piece isPieceRemoved; /**< Pointer to removed piece, null otherwise. */
} def_move;

/**
 * Prints move data in protocol specified
 * by game. Does nothing if move command type
 * is DO_NOTHING.
 * @param[in] move Move to be printed.
 */
void printMoveCommand(Move move);

/**
 * Makes a move on board. Move is required
 * to be valid.
 * @param[in] Board
 * @param[in] move
 */
void doMove(board Board, Move move);

/**
 * Undo a previusly performed move on board.
 */
void undoMove(board, Move);

/**
 * Returns best move for a piece in the current
 * state of the game.
 * @param[in] Board Current state of the board.
 * @param[in] piece Piece which move is searched.
 * @param[in] piecePosition Piece position, it is
 *            necessary because do not remember
 *            their position on board.
 */
Move getBestMove(board Board, Piece piece, Point piecePosition);

/**
 * Returns fresh move which ends turn.
 */
Move endTurnMove();

/**
 * Returns list of valid moves a piece can perform.
 */
int getPossibleMoves(board, Piece, Move*, Point);

Move newMove();

/**
 * Frees move completely from memory.
 */
void cleanMoveData(Move);

#endif //MIDDLE_AGES_MOVE_H
