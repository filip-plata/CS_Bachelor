#include <cassert>
#include "../citizen.h"
#include "../monster.h"
#include "../smalltown.h"
#include <iostream>
template<typename Functor>
void tickAndCheckStatus(SmallTown &smallTown, int time, Functor fun) {
    smallTown.tick(time);
    fun(smallTown.getStatus());
}

void orderTests1() {
    auto monster = createVampire(200, 20);
    auto sheriff = createSheriff(200, 40, 1000);
    auto c1 = createAdult(10, 50);
    auto c2 = createAdult(10, 20);
    auto smallTown = SmallTown::Builder()
        .monster(monster)
        .startTime(2)
        .startTime(3)
        .maxTime(20)
        .citizen(sheriff)
        .citizen(c1)
        .citizen(c2)
        .build();
    smallTown.tick(1);
    assert(smallTown.getStatus().getAliveCitizens() == 3);
}
void orderTests2() {
    auto monster = createVampire(200, 20);
    auto sheriff = createSheriff(200, 40, 1000);
    auto c1 = createAdult(10, 50);
    auto c2 = createAdult(10, 20);
    auto smallTown = SmallTown::Builder()
            .monster(monster)
            .startTime(2)
            .startTime(3)
            .maxTime(20)
            .citizen(c1)
            .citizen(sheriff)
            .citizen(c2)
            .build();
    smallTown.tick(1);
    assert(smallTown.getStatus().getAliveCitizens() == 2);
}
void orderTests3() {
    auto monster = createVampire(200, 20);
    auto sheriff = createSheriff(200, 40, 1000);
    auto c1 = createAdult(10, 50);
    auto c2 = createAdult(10, 20);
    auto smallTown = SmallTown::Builder()
            .monster(monster)
            .startTime(2)
            .startTime(3)
            .maxTime(20)
            .citizen(c1)
            .citizen(c2)
            .citizen(sheriff)
            .build();
    smallTown.tick(1);
    assert(smallTown.getStatus().getAliveCitizens() == 1);
}
void orderTests() {
    orderTests1();
    orderTests2();
    orderTests3();
}

void testSheriffResponseOnSingleMonster() {
    auto monster = createMummy(20, 10);
    auto smallTown = SmallTown::Builder()
        .monster(monster)
        .startTime(3)
        .maxTime(27)
        .citizen(createSheriff(100, 35, 10))
        .build();

    // First round, monster takes hit.
    tickAndCheckStatus(smallTown, 3, [](auto status) { assert(status.getMonsterHealth() == 10); });
     // Monster should be dead, sheriff alive.
    tickAndCheckStatus(smallTown, 3, [](auto status) {
        assert(status.getMonsterHealth() == 0);
        assert(status.getAliveCitizens() == 1);
    });
}

// Macro is used here so assert error will show real line number
#define tickAndCheckMonsterHealth(A, B, C) tickAndCheckStatus((A), (B), [](auto status) { assert(status.getMonsterHealth() == (C)); });

void testAttackShouldHappenOnlyOnSpecificTime() {
    auto monster = createMummy(100, 1);
    auto smallTown = SmallTown::Builder()
        .monster(monster)
        .startTime(3)
        .maxTime(27)
        .citizen(createSheriff(100, 35, 1))
        .build();
                                                 // time before attack
    tickAndCheckMonsterHealth(smallTown, 1, 99); // 3 attack
    tickAndCheckMonsterHealth(smallTown, 1, 99); // 4
    tickAndCheckMonsterHealth(smallTown, 1, 99); // 5
    tickAndCheckMonsterHealth(smallTown, 1, 98); // 6 attack
    tickAndCheckMonsterHealth(smallTown, 3, 98); // 7 !
    tickAndCheckMonsterHealth(smallTown, 3, 98); // 10
    tickAndCheckMonsterHealth(smallTown, 8, 97); // 13 attack
    tickAndCheckMonsterHealth(smallTown, 1, 97); // 21 !
}

void testAttack() {
    testSheriffResponseOnSingleMonster();
    testAttackShouldHappenOnlyOnSpecificTime();
}

void testFromExample() {
  auto groupOfMonsters = createGroupOfMonsters({
        createMummy(90, 1),
        createZombie(20, 1),
        createVampire(30, 1)
    });
    auto smallTown = SmallTown::Builder()
        .monster(groupOfMonsters)
        .startTime(3)
        .maxTime(27)
        .citizen(createSheriff(100, 35, 20))
        .citizen(createAdult(100, 21))
        .citizen(createTeenager(50, 14))
        .build();

    smallTown.tick(18);
    smallTown.tick(3);

    auto status = smallTown.getStatus();
    assert(status.getMonsterName() == "GroupOfMonsters");
    assert(status.getMonsterHealth() == 80);
    assert(status.getAliveCitizens() == 3);
}

int main() {
    testAttack();
    testFromExample();
    orderTests();
}
