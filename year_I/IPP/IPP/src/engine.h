/** @file
    Interface of game engine.
 */

#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

/**
 * Initializes a game. Needed before first INIT.
 */
void startGame();

/**
 * Frees memory. Needed after finishing game.
 */
void endGame();

/**
 * Initializes a game with size of a board, number of rounds and positions of kings.
 */
int init(int n, int k, int p, int x1, int y1, int x2, int y2);

/**
 * Makes a move.
 * @param[in] x1 Column number before a move.
 * @param[in] y1 Row number before a move.
 * @param[in] x2 Column number after a move.
 * @param[in] y2 Row number before a move.
 * @return Returns 1 on failure and 0 otherwise.
 */
int move(int x1, int y1, int x2, int y2);

/**
 * Produces a knight.
 * @param[in] x1 Column number of the father-peasant.
 * @param[in] y1 Row number of the father-peasant.
 * @param[in] x2 Column number of created knight.
 * @param[in] y2 Row number of created knight.
 * @return Returns 1 on failure and 0 otherwise.
 */
int produceKnight(int x1, int y1, int x2, int y2);

/**
 * Produces a peasant.
 * @param[in] x1 Column number of the father-peasant.
 * @param[in] y1 Row number of the father-peasant.
 * @param[in] x2 Column number of created peasant.
 * @param[in] y2 Row number of created peasant.
 * @return Returns 1 on failure and 0 otherwise.
 */
int producePeasant(int x1, int y1, int x2, int y2);

/**
 * Ends player's turn.
 */
int endTurn();

/**
 * @return True if game is over, false otherwise.
 */
bool gameOver();

/**
 * Returns result of the game. 0 is win, 1 is a draw and 2 stands for losing.
 * */
int gameResult();

/**
 * Prints to stdout moves to perform in AI turn.
 * */
void performMoves();


#endif /* ENGINE_H */
