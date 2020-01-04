#include <cassert>
#include <cstdio>
#include "../monster.h"

void zombieTest() {
    Zombie<int> zombie(15, 15);
    /* Constructor Tests */
    assert(zombie.getHealth() == 15);
    assert(zombie.getAttackPower() == 15);

    /* Take damage when health >= damage */
    zombie.takeDamage(10);
    assert(zombie.getHealth() == 5);

    /* Take damage when health <= damage */
    zombie.takeDamage(10);
    assert(zombie.getHealth() == 0);

    /* valueType Test */
    if (!std::is_same<int, Zombie<int>::valueType>::value) {
        assert("!std::is_base_of<int, Zombie<int>::valueType>::value" && false);
    }

    if (std::is_same<float, Zombie<int>::valueType>::value) {
        assert("std::is_base_of<float, Zombie<int>::valueType>::value" && false);
    }
}

void vampireTest() {
    Vampire<int> vampire(15, 15);
    /* Constructor Tests */
    assert(vampire.getHealth() == 15);
    assert(vampire.getAttackPower() == 15);

    /* Take damage when health >= damage */
    vampire.takeDamage(10);
    assert(vampire.getHealth() == 5);

    /* Take damage when health <= damage */
    vampire.takeDamage(10);
    assert(vampire.getHealth() == 0);

    /* valueType Test */
    if (!std::is_same<int, Vampire<int>::valueType>::value) {
        assert("!std::is_base_of<int, Zombie<int>::valueType>::value" && false);
    }

    if (std::is_same<float, Vampire<int>::valueType>::value) {
        assert("std::is_base_of<float, Zombie<int>::valueType>::value" && false);
    }
}

void mummyTest() {
    Mummy<int> mummy(15, 15);
    /* Constructor Tests */
    assert(mummy.getHealth() == 15);
    assert(mummy.getAttackPower() == 15);

    /* Take damage when health >= damage */
    mummy.takeDamage(10);
    assert(mummy.getHealth() == 5);

    /* Take damage when health <= damage */
    mummy.takeDamage(10);
    assert(mummy.getHealth() == 0);

    /* valueType Test */
    if (!std::is_same<int, Mummy<int>::valueType>::value) {
        assert("!std::is_base_of<int, Zombie<int>::valueType>::value" && false);
    }

    if (std::is_same<float, Mummy<int>::valueType>::value) {
        assert("std::is_base_of<float, Zombie<int>::valueType>::value" && false);
    }
}

int main() {
    printf("Monster tests\n");
    printf("  * Zombie...       ");
    zombieTest();
    printf(" OK.\n");
    printf("  * Vampire...      ");
    vampireTest();
    printf(" OK.\n");
    printf("  * Mummy...        ");
    mummyTest();
    printf(" OK.\n");
}