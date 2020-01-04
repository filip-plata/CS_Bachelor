#include "../monster.h"
#include "../citizen.h"
#include <cassert>

void test_group() {
    auto group = 
	createGroupOfMonsters(
                              { createVampire(100, 10),
                                      createZombie(200, 20),
                                      createMummy(300, 30)});
 
    assert(group->getHealth() == 600);
    assert(group->getAttackPower() == 60);
    
    group->takeDamage(100);
    assert(group->getHealth() == 300);
    assert(group->getAttackPower() == 50);

    group->takeDamage(100);
    assert(group->getHealth() == 100);
    assert(group->getAttackPower() == 30);
    
    group->takeDamage(100);
    assert(group->getHealth() == 0);
    assert(group->getAttackPower() == 0);
}


void test_attack_civilian() {
    auto group = 
	createGroupOfMonsters(
                              { createVampire(100, 10),
                                      createZombie(200, 20),
                                      createMummy(300, 30)});

    Teenager teenager(8, 15);
    teenager.attack(*group);
    
    assert(teenager.getHealth() == 0);
    assert(group->getHealth() == 600);
    
    Adult adult(8, 20);
 
    adult.attack(*group);
    
    assert(adult.getHealth() == 0);
    assert(group->getHealth() == 600);
}


void test_attack_sheriff() {
    auto group = 
	createGroupOfMonsters(
                              { createVampire(10, 10),
                                createZombie(20, 20),
                                createMummy(30, 30)});
    
    Sheriff sheriff(200, 20, 10);
    sheriff.attack(*group);
    assert(sheriff.getHealth() == 140);
    assert(group->getHealth() == 30);
    
    sheriff.attack(*group);
    assert(group->getHealth() == 10);
    assert(sheriff.getHealth() == 90);
    
    sheriff.attack(*group);
    assert(group->getHealth() == 0);
    assert(sheriff.getHealth() == 60);
}

			
int main() {
    test_group();
    test_attack_civilian();
    test_attack_sheriff();

    return 0;
}
