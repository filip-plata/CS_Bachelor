/** @file board.h
 * Board interface used to implement
 * engine functionality.
 *
 * */
#include <stdbool.h>
#include "piece.h"
#include "point.h"

#ifndef MIDDLE_AGES_BOARD_H
#define MIDDLE_AGES_BOARD_H


/**
 * struct defBoard can be implemented
 * in way independent of provided functionality.
 */
typedef struct defBoard *board;

/**
 * Creates new instance of board structure.
 * Must be called before any other function.
 */
board newGameBoard();

/**
 * Passes into a board some data about game.
 * @param[in] Board Board to which data is passed.
 * @param[in] size Size of the board.
 * @param[in] turns Game duration.
 */
void setGameValues(board Board, int size, int turns, int, int, int, int);

/**
 * Returns piece from a position or NULL if
 * the field is not occupied.
 *  @param[in] Board Board which is asked about a position.
 *  @param[in] x Column number.
 *  @param[in] y Row number.
 */
Piece pieceOnPosition(board Board, int x, int y);

/**
 * Removes piece from a position or do not performs
 * action if the field is not occupied.
 * @param[in] Board Board with piece to be removed.
 * @param[in] x Column number.
 * @param[in] y Row number.
 */
void removePiece(board Board, int x, int y);

/**
 * Inserts piece to a specified position into board.
 * @param[in] Board Board to insert piece.
 * @param[in] x Column number.
 * @param[in] y Row number.
 * @param[in] piece Piece to be inserted.
 */
void putPiece(board Board, int x, int y, Piece piece);

/**
 * Returns whose player turn it is.
 * @param[in] Board Board.
 */
int currentPlayerNumber(board Board);

/**
 * Returns number of current turn.
 * @param[in] Board Board.
 */
int currentTurn(board Board);

/**
 * Informs board that a new turn has begun.
 * @param[in] Board Board.
 */
void incrementTurn(board Board);

/**
 * Switches current player on a given board.
 * @param[in] Board Board.
 */
void updateCurrentPlayerNumber(board Board);

/**
 * Informs whether a position is occupied by any piece.
 * @param[in] Board Board which we inspect.
 * @param[in] x Column number.
 * @param[in] y Row number.
 */
bool isPositionFree(board Board, int x, int y);

/**
 * Destructor of board. Frees memory completely.
 * @param[in] Board Board to be destroyed.
 */
void deleteGameBoard(board Board);

/**
 * Returns size of the board.
 * @param[in] Board Board.
 */
int boardSize(board Board);

/**
 * Returns limit on the number of turns.
 * @param[in] Board Board.
 */
int turnsLimit(board Board);

/**
 * Returns 1 if king of player is alive and 0
 * otherwise on a given board.
 * @param[in] Board Board to be asked.
 * @param[in] player Player number of player asked.
 */
int getKing(board Board, int player);

/**
 * Informs a given board that king of a player
 * has been captured.
 * @param[in] Board Board.
 * @param[in] player Number of unfortunate player.
 */
void killKing(board Board, int player);

/**
 * Returns pointer to a struct holding player's
 * king row and column number.
 */
Point getKingPosition(board Board, int player);

/**
 * Returns a deep copy of a board.
 */
board boardCopy(board Board);

/**
 * Changes the peasants number.
 * @param[in] number Number of new peasants.
 */
void incrementPeasants(board x, int number);

/**
 * Changes the knights number.
 * @param[in] number Number of new knights.
 */
void incrementKnights(board x, int number);

/**
 * Returns number of peasants on board.
 */
int getPeasants(board x);

/**
 * Returns number of knights on board.
 */
int getKnights(board x);

#endif //MIDDLE_AGES_BOARD_H
