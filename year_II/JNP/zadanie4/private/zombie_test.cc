#include "../monster.h"

int main() {
    /* Uncomment to test [should not compile */

    // 1. Non arithmetic type

    class Dummy {};
    Zombie<Dummy> zombie(Dummy(), Dummy());


    // 2. Illegal health
    /*
    Zombie<int> zombie(0, 20);
    */

    // 3. Illegal attack power
    /*
    Zombie<int> zombie(20, 0);
    */
}