#include "../citizen.h"

int main() {
    /* Uncomment to test [should not compile */

    // 1. Non arithmetic type
    /*
    class Dummy {};
    Teenager<Dummy> teenager(Dummy(), Dummy());
    */

    // 2. Illegal age

    Teenager<int> teenager(20, 20);


    // 3. Illegal health
    /*
    Teenager<int> teenager(0, 12);
    */

    // 4. Attempt to use forbidden constructor
    /*
    Teenager<int> teenager(15, 15, 20);
    */

    // 5. Attempt to use forbidden function
    /*
    Teenager<int> teenager(15, 15);
    teenager.getAttackPower();
    */
}