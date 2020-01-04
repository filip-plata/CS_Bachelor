#include "../monster.h"
#include "../citizen.h"
#include <cassert>


void test_vampire() {
    Vampire vampire(100, 20);

    assert(vampire.getHealth() == 100);
    assert(vampire.getAttackPower() == 20);
}

void test_mummy() {
    Mummy mummy(70, 20);

    assert(mummy.getHealth() == 70);
    assert(mummy.getAttackPower() == 20);
}

void test_zombie() {
    Zombie zombie(30, 100000);

    assert(zombie.getHealth() == 30);
    assert(zombie.getAttackPower() == 100000);
    
    zombie.takeDamage(20);
    
    assert(zombie.getHealth() == 10);

    zombie.takeDamage(20);
    
    assert(zombie.getHealth() == 0);

    zombie.takeDamage(178);
    
    assert(zombie.getHealth() == 0);
}

void test_attack_sheriff() {
    Vampire vampire(100, 20);
    Sheriff sheriff(100, 20, 50);

    sheriff.attack(vampire);

    assert(sheriff.getHealth() == 80);
    assert(vampire.getHealth() == 50);

    sheriff.attack(vampire);

    assert(sheriff.getHealth() == 60);
    assert(vampire.getHealth() == 0);

    Mummy mummy(40, 20);

    sheriff.attack(mummy);

    assert(sheriff.getHealth() == 40);
    assert(mummy.getHealth() == 0);

    Zombie zombie(49, 20);

    sheriff.attack(zombie);

    assert(sheriff.getHealth() == 20);
    assert(zombie.getHealth() == 0);

}

void test_attack_civilian() {
    Vampire vampire(100, 20);
    Teenager teenager(8, 15);

    teenager.attack(vampire);

    assert(teenager.getHealth() == 0);
    assert(vampire.getHealth() == 100);

    Mummy mummy(20, 20);
    Adult adult(8, 20);

    adult.attack(mummy);

    assert(adult.getHealth() == 0);
    assert(mummy.getHealth() == 20);
}


int main() {
    test_vampire();
    test_mummy();
    test_zombie();
    test_attack_sheriff();
    test_attack_civilian();
    
    return 0;
}
