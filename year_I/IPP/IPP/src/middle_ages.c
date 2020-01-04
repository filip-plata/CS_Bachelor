#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "parse.h"
#include "engine.h"
#include "utils.h"


int main() {
    startGame();

    bool my_turn = false;
    command *new_command;

    while (1) {
        new_command = parse_command();
        int ret = 0;

        if (new_command == NULL) {
            fprintf(stderr, "input error\n");
            endGame();
            return 42;
        }
        
        if (strcmp(new_command->name, "INIT") == 0) {
            ret = init(new_command->data[0],
                       new_command->data[1],
                       new_command->data[2],
                       new_command->data[3],
                       new_command->data[4],
                       new_command->data[5],
                       new_command->data[6]);
            if (new_command->data[2] == 1) {
                my_turn = true;
            }
        }

        if (strcmp(new_command->name, "MOVE") == 0) {
            ret = move(new_command->data[0],
                       new_command->data[1],
                       new_command->data[2],
                       new_command->data[3]);
        }

        if (strcmp(new_command->name, "PRODUCE_KNIGHT") == 0) {
            ret = produceKnight(new_command->data[0],
                                new_command->data[1],
                                new_command->data[2],
                                new_command->data[3]);
        }

        if (strcmp(new_command->name, "PRODUCE_PEASANT") == 0) {
            ret = producePeasant(new_command->data[0],
                                 new_command->data[1],
                                 new_command->data[2],
                                 new_command->data[3]);
        }

        if (strcmp(new_command->name, "END_TURN") == 0) {
            ret = endTurn();
            my_turn = true;
        }

        if (ret) {
            free(new_command);
            fprintf(stderr, "input error\n");
            endGame();
            return 42;
        }

        if (my_turn) {
            performMoves();
            fflush(stdout);
        }

        my_turn = false;
        free(new_command);

        if (gameOver()) {
            break;
        }
    }

    int game_over = gameResult();
    endGame();

    return game_over;
}
