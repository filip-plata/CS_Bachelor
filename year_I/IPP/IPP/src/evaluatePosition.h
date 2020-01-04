
/**
 * @file
 * Header providing tools for evaluating
 * position on board.
 */

#ifndef MIDDLE_AGES_EVALUATEPOSITION_H
#define MIDDLE_AGES_EVALUATEPOSITION_H

#include "board.h"
#include "move.h"

/**
 * Evaluates position from a perspective of player.
 * Returned value means how "good" or "bad" a position is.
 * It scales with size of the board in a meaningful range up to
 * board size equal 30.
 * @param[in] Game Game to be evaluated.
 * @param[in] player Player for whom game status
 *                    is evaluated.
 */
int evaluatePosition(board Game, int player, Move move);


#endif //MIDDLE_AGES_EVALUATEPOSITION_H
