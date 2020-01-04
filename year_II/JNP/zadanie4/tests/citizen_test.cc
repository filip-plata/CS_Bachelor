#include "../citizen.h"
#include <cassert>

void test_sheriff() {
    Sheriff<int> sheriff(100, 50, 20);

    assert(sheriff.getHealth() == 100);
    assert(sheriff.getAge() == 50);
    assert(sheriff.getAttackPower() == 20);

    sheriff.takeDamage(20);

    assert(sheriff.getHealth() == 80);
}


void test_adult() {
    Adult<long long> adult(10, 18);

    assert(adult.getHealth() == 10);
    assert(adult.getAge() == 18);

    adult.takeDamage(20);

    assert(adult.getHealth() == 0);    
}

void test_teenager() {
    Teenager<unsigned long> teenager(21, 12);

    assert(teenager.getHealth() == 21);
    assert(teenager.getAge() == 12);

    teenager.takeDamage(20);

    assert(teenager.getHealth() == 1);
}


int main() {
    test_sheriff();
    test_teenager();
    test_adult();
    
    return 0;
}
