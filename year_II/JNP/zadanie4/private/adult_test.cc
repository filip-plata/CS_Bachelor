#include "../citizen.h"

int main() {
    /* Uncomment to test [should not compile */

    // 1. Non arithmetic type
    /*
    class Dummy {};
    Adult<Dummy> adult(Dummy(), Dummy());
    */

    // 2. Illegal age
    /*
    Adult<int> adult(15, 15);
    */

    // 3. Illegal health
    /*
    Adult<int> adult(0, 20);
    */

    // 4. Attempt to use forbidden constructor

    Adult<int> adult(20, 20, 20);


    // 5. Attempt to use forbidden function
    /*
    Adult<int> adult(20, 20);
    adult.getAttackPower();
    */
}