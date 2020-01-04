#include <stdio.h>
#include <stdarg.h>
#include <setjmp.h>
#include <stdbool.h>
#include "cmocka.h"

extern void startGame();
extern void endGame();
extern bool gameOver();
extern void endTurn();
extern void performMoves();
extern int middle_ages_main();
extern int init(int n, int k, int p, int x1, int y1, int x2, int y2);


void test_middle_ages_idle_ai_board_10(void **state) {
    startGame();

    init(10, 20, 1, 1, 1, 1, 10);

    int k= 0;

    while (!gameOver()) {
        performMoves();
        endTurn();
        endTurn();
        k++;
    }

    endGame();

    assert_in_range(k, 0, 20);
}

void test_middle_ages_idle_ai_board_20(void **state) {
    startGame();

    init(20, 40, 1, 1, 1, 1, 20);

    int k= 0;

    while (!gameOver()) {
        performMoves();
        endTurn();
        endTurn();
        k++;
    }

    endGame();

    assert_in_range(k, 0, 40);
}

void test_middle_ages_idle_ai_board_30(void **state) {
    startGame();

    init(30, 60, 1, 1, 1, 1, 30);

    int k= 0;

    while (!gameOver()) {
        performMoves();
        endTurn();
        endTurn();
        k++;
    }

    endGame();

    assert_in_range(k, 0, 59);
}

void test_middle_ages_idle_ai_board_200(void **state) {
    startGame();

    init(200, 1000, 1, 1, 1, 1, 200);

    int k= 0;

    while (!gameOver()) {
        performMoves();
        endTurn();
        endTurn();
        k++;
    }

    endGame();

    assert_in_range(k, 0, 400);
}


int main(void) {
    const struct CMUnitTest tests[] = {
            cmocka_unit_test(test_middle_ages_idle_ai_board_10),
            cmocka_unit_test(test_middle_ages_idle_ai_board_20),
            cmocka_unit_test(test_middle_ages_idle_ai_board_30),
            cmocka_unit_test(test_middle_ages_idle_ai_board_200),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
