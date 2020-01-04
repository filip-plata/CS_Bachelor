#include <cassert>
#include <iostream>
#include "../citizen.h"
#include "../monster.h"
#include "../smalltown.h"

void attacking_test() {
    auto smallTown = SmallTown::Builder()
        .monster(createMummy(5, 1))
        .startTime(3)
        .maxTime(27)
        .citizen(createSheriff(100, 35, 20))
        .build();

    smallTown.tick(18);

    auto status = smallTown.getStatus();
    assert(status.getMonsterName() == "Mummy");
    assert(status.getMonsterHealth() == 0);
    assert(status.getAliveCitizens() == 1);

    auto group = createGroupOfMonsters({
            createZombie(90, 50),
            createVampire(35, 10)
    });

    smallTown = SmallTown::Builder()
        .monster(group)
        .startTime(3)
        .maxTime(27)
        .citizen(createSheriff(100, 35, 35))
        .citizen(createTeenager(60, 14))
        .build();

    smallTown.tick(18);
    status = smallTown.getStatus();
    assert(status.getMonsterName() == "GroupOfMonsters");
    assert(status.getMonsterHealth() == 55);
    assert(status.getAliveCitizens() == 2);
    smallTown.tick(3);
    smallTown.tick(3);
    
    status = smallTown.getStatus();
    assert(status.getMonsterName() == "GroupOfMonsters");
    assert(status.getMonsterHealth() == 20);
    assert(status.getAliveCitizens() == 0);
}

int main(int argc, const char * argv[]) {
    attacking_test();
    return 0;
}
