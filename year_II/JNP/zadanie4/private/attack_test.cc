#include <cstdio>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "../citizen.h"
#include "../monster.h"

template <typename T, typename U>
void zombieTeenager(T zombieHealth, T zombieAttack, U teenagerHealth) {
    Zombie<T> zombie(zombieHealth, zombieAttack);
    Teenager<U> teenager(teenagerHealth, 12);

    while (teenagerHealth > zombieAttack) {
        teenagerHealth -= static_cast<U>(zombieAttack);
        attack(zombie, teenager);
        assert(teenager.getHealth() == teenagerHealth);
    }

    attack(zombie, teenager);
    assert(teenager.getHealth() == 0);
}

template <typename T, typename U>
void zombieSheriff(T zombieHealth, T zombieAttack, U sheriffHealth, U sheriffAttack, int win) {
    Zombie<T> zombie(zombieHealth, zombieAttack);
    Sheriff<U> sheriff(sheriffHealth, 20, sheriffAttack);

    while (sheriffHealth > zombieAttack && zombieHealth > sheriffAttack) {
        sheriffHealth = std::max(sheriffHealth - static_cast<U>(zombieAttack), static_cast<U>(0));
        zombieHealth = std::max(zombieHealth - static_cast<T>(sheriffAttack), static_cast<T>(0));

        attack(zombie, sheriff);
        assert(sheriff.getHealth() == sheriffHealth);
        assert(zombie.getHealth() == zombieHealth);
    }

    attack(zombie, sheriff);
    sheriffHealth = std::max(sheriffHealth - static_cast<U>(zombieAttack), static_cast<U>(0));
    zombieHealth = std::max(zombieHealth - static_cast<T>(sheriffAttack), static_cast<T>(0));
    switch (win) {
        case -1: assert(sheriff.getHealth() == 0 && zombie.getHealth() > 0); break;
        case 0: assert(sheriff.getHealth() == 0 && zombie.getHealth() == 0); break;
        case 1: assert(sheriff.getHealth() > 0 && zombie.getHealth() == 0); break;
        default: assert(false);
    }
}

int main() {
    printf("Attack tests\n");

    printf("  * Zombie<int> vs Teenager<int> [1 hit KO]...               ");
    zombieTeenager<int, int>(10, 10, 10);
    printf(" OK.\n");

    printf("  * Zombie<int> vs Teenager<int> [2 hit KO]...               ");
    zombieTeenager<int, int>(10, 10, 20);
    printf(" OK.\n");

    printf("  * Zombie<int> vs Teenager<int> [2.5 hit KO]...             ");
    zombieTeenager<int, int>(10, 10, 25);
    printf(" OK.\n");

    printf("  * Zombie<float> (int) vs Teenager<int> [1 hit KO]...        ");
    zombieTeenager<float, int>(10.f, 10.f, 10);
    printf(" OK.\n");

    printf("  * Zombie<float> (int) vs Teenager<int> [2 hit KO]...        ");
    zombieTeenager<float, int>(10.f, 10.f, 20);
    printf(" OK.\n");

    printf("  * Zombie<float> (int) vs Teenager<int> [2.5 hit KO]...      ");
    zombieTeenager<float, int>(10.f, 10.f, 25);
    printf(" OK.\n");

    printf("  * Zombie<float> (float) vs Teenager<int> [2.1 hit KO]...      ");
    zombieTeenager<float, int>(9.5f, 9.5f, 20);
    printf(" OK.\n");


    printf("  * Zombie<double> (int) vs Teenager<int> [1 hit KO]...        ");
    zombieTeenager<double, int>(10., 10., 10);
    printf(" OK.\n");

    printf("  * Zombie<double> (int) vs Teenager<int> [2 hit KO]...        ");
    zombieTeenager<double, int>(10., 10., 20);
    printf(" OK.\n");

    printf("  * Zombie<double> (int) vs Teenager<int> [2.5 hit KO]...      ");
    zombieTeenager<double, int>(10., 10., 25);
    printf(" OK.\n");

    printf("  * Zombie<double> (int) vs Teenager<int> [2.1 hit KO]...      ");
    zombieTeenager<double, int>(9.5, 9.5, 20);
    printf(" OK.\n");

    printf("  * Zombie<int> vs Sheriff<int> [DRAW]...      ");
    zombieSheriff<int, int>(10, 10, 10, 10, 0);
    printf(" OK.\n");

    printf("  * Zombie<int> vs Sheriff<int> [Sheriff WIN]...      ");
    zombieSheriff<int, int>(10, 10, 20, 10, 1);
    printf(" OK.\n");

    printf("  * Zombie<int> vs Sheriff<int> [Zombie WIN]...      ");
    zombieSheriff<int, int>(20, 20, 10, 10, -1);
    printf(" OK.\n");
}
