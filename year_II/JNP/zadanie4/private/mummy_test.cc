#include "../monster.h"

int main() {
    /* Uncomment to test [should not compile */

    // 1. Non arithmetic type
    /*
    class Dummy {};
    Mummy<Dummy> mummy(Dummy(), Dummy());
    */

    // 2. Illegal health

    Mummy<int> mummy(0, 20);


    // 3. Illegal attack power
    /*
    Mummy<int> mummy(20, 0);
    */
}