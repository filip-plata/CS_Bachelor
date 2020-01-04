#include <cassert>
#include "../generator.h"
#include "test_generator.h"


void test_generator() {
    gen_t value, seed = 3545464864;
    set_seed(seed);

    value = next();
    assert(value == 3545464864);

    value = next();
    assert(value == 4223707516);

    value = next();
    assert(value == 1461086045);

    value = next();
    assert(value == 3489936252);
}
