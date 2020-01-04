#include "../monster.h"

int main() {
    /* Uncomment to test [should not compile */

    // 1. Non arithmetic type
    /*
    class Dummy {};
    Vampire<Dummy> vampire(Dummy(), Dummy());
    */

    // 2. Illegal health
    /*
    Vampire<int> vampire(0, 20);
    */

    // 3. Illegal attack power

    Vampire<int> vampire(20, 0);

}