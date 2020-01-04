#include "../monster.h"
#include "../citizen.h"
#include <cassert>


void test_vampire() {
    Vampire<int> vampire(100, 20);

    assert(vampire.getHealth() == 100);
    assert(vampire.getAttackPower() == 20);
}

void test_mummy() {
    Mummy<float> mummy(70., 20.);

    assert(mummy.getHealth() == 70.);
    assert(mummy.getAttackPower() == 20.);
}

void test_zombie() {
    Zombie<long long> zombie(30, 100000);

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
    Vampire<int> vampire(100, 20);
    Sheriff<int> sheriff(100, 20, 50);

    attack(vampire, sheriff);

    assert(sheriff.getHealth() == 80);
    assert(vampire.getHealth() == 50);

    attack(vampire, sheriff);

    assert(sheriff.getHealth() == 60);
    assert(vampire.getHealth() == 0);

    Mummy<long> mummy(40, 20);

    attack(mummy, sheriff);

    assert(sheriff.getHealth() == 40);
    assert(mummy.getHealth() == 0);

    Zombie<unsigned long> zombie(49, 20);

    attack(zombie, sheriff);

    assert(sheriff.getHealth() == 20);
    assert(zombie.getHealth() == 0);

}

void test_attack_civilian() {
    Vampire<unsigned int> vampire(100, 20);
    Teenager<unsigned int> teenager(8, 15);

    attack<Vampire<unsigned int>, Teenager<unsigned int> >(vampire, teenager);

    assert(teenager.getHealth() == 0);
    assert(vampire.getHealth() == 100);

    Mummy<unsigned int> mummy(20, 20);
    Adult<unsigned long long> adult(8, 20);

    attack<Mummy<unsigned int>, Adult<unsigned long long> >(mummy, adult);

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
