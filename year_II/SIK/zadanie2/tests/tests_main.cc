#include "test_all.h"


int main(int argc, char *argv[]) {
    #ifndef GENERIC_TEST

    #endif /* GENERIC_TEST */

    test_utils();
    test_generator();
    test_event();
    test_communicate();

    return 0;
}
