
/**
 * @file
 * Interface for fetching AI orders.
 */

#include "board.h"
#include "move.h"

#ifndef MIDDLE_AGES_AI_H
#define MIDDLE_AGES_AI_H

/**
 * Number of the ai player. Initialized by init in engine.
 */
int aiPlayerNumber;

/**
 * Returns ai orders for the current state of game.
 * Algorithm is greedy, main core is position evaluation.
 * @param[in] Board Board.
 * @return Returns string of orders.
 */
int fetchAiCommands(board Board, Move*);


#endif //MIDDLE_AGES_AI_H
