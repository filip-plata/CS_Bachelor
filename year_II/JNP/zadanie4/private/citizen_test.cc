#include <cassert>
#include <cstdio>
#include "../citizen.h"

void teenagerTest() {
    /* Integer */
    Teenager<int> teenager(15, 15);
    /* Constructor Tests */
    assert(teenager.getAge() == 15);
    assert(teenager.getHealth() == 15);

    /* Take damage when health >= damage */
    teenager.takeDamage(10);
    assert(teenager.getHealth() == 5);

    /* Take damage when health <= damage */
    teenager.takeDamage(10);
    assert(teenager.getHealth() == 0);

    /* Double */
    Teenager<double> teenagerDouble(15, 15);
    /* Constructor Tests */
    assert(teenagerDouble.getAge() == 15);
    assert(teenagerDouble.getHealth() == 15);

    /* Take damage when health >= damage */
    teenagerDouble.takeDamage(10);
    assert(teenagerDouble.getHealth() == 5);

    /* Take damage when health <= damage */
    teenagerDouble.takeDamage(10);
    assert(teenagerDouble.getHealth() == 0);
}

void adultTest() {
    /* Integer */
    Adult<int> adult(20, 20);

    /* Constructor Tests */
    assert(adult.getAge() == 20);
    assert(adult.getHealth() == 20);

    /* Take damage when health >= damage */
    adult.takeDamage(15);
    assert(adult.getHealth() == 5);

    /* Take damage when health <= damage */
    adult.takeDamage(15);
    assert(adult.getHealth() == 0);

    /* Double */
    Adult<double> adultDouble(20, 20);

    /* Constructor Tests */
    assert(adultDouble.getAge() == 20);
    assert(adultDouble.getHealth() == 20);

    /* Take damage when health >= damage */
    adultDouble.takeDamage(15);
    assert(adultDouble.getHealth() == 5);

    /* Take damage when health <= damage */
    adultDouble.takeDamage(15);
    assert(adultDouble.getHealth() == 0);
}

void sheriffTest() {
    /* Integer */
    Sheriff<int> sheriff(20, 20, 20);

    /* Constructor Tests */
    assert(sheriff.getAge() == 20);
    assert(sheriff.getHealth() == 20);
    assert(sheriff.getAttackPower() == 20);

    /* Take damage when health >= damage */
    sheriff.takeDamage(15);
    assert(sheriff.getHealth() == 5);

    /* Take damage when health <= damage */
    sheriff.takeDamage(15);
    assert(sheriff.getHealth() == 0);

    /* Double */
    Sheriff<double> sheriffDouble(20, 20, 20);

    /* Constructor Tests */
    assert(sheriffDouble.getAge() == 20);
    assert(sheriffDouble.getHealth() == 20);
    assert(sheriffDouble.getAttackPower() == 20);

    /* Take damage when health >= damage */
    sheriffDouble.takeDamage(15);
    assert(sheriffDouble.getHealth() == 5);

    /* Take damage when health <= damage */
    sheriffDouble.takeDamage(15);
    assert(sheriffDouble.getHealth() == 0);
}

int main() {
    printf("Citizen tests\n");
    printf("  * Teenager...     ");
    teenagerTest();
    printf(" OK.\n");
    printf("  * Adult...        ");
    adultTest();
    printf(" OK.\n");
    printf("  * Sheriff...      ");
    sheriffTest();
    printf(" OK.\n");
}