#include "../citizen.h"

int main() {
    /* Uncomment to test [should not compile */

    // 1. Non arithmetic type
    /*
    class Dummy {};
    Sheriff<Dummy> sheriff(Dummy(), Dummy(), Dummy());
    */

    // 2. Illegal age
    /*
    Sheriff<int> sheriff(15, 15, 15);
    */

    // 3. Illegal health
    /*
    Sheriff<int> sheriff(0, 20, 20);
    */

    // 4. Illegal attack power
    /*
    Sheriff<int> sheriff(20, 20, 0);
    */

    // 5. Attempt to use forbidden constructor

    Sheriff<int> sheriff(20, 20);

}
