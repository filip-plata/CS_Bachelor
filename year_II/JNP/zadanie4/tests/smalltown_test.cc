#include "../smalltown.h"
#include <cassert>
#include <iostream>

void no_fibonacci_ticking() {
    auto smalltown = SmallTown<Zombie<int>, int, 4, 100, Sheriff<long long>>(
                                                                             Zombie<int>(20, 1),
                                                                             Sheriff<long long>(1, 50, 2000)

                                                                          

                                                                             );

    // time 4
    smalltown.tick(2);
    // time 6
    smalltown.tick(1);
    // time 7
    smalltown.tick(2);
    // time 7
    smalltown.tick(5);
    // time 12
    smalltown.tick(85);
    // time 97

    auto status = smalltown.getStatus();

    assert( std::get<0>(status) != "");
    assert( std::get<1>(status) == 20);
    assert( std::get<2>(status) == 1);
}

void monster_dies_after_attack() {
    auto smalltown = SmallTown<Zombie<int>, int, 1, 7, Sheriff<long long>, Teenager<int>>(
                                                                             Zombie<int>(20, 7),
                                                                             Sheriff<long long>(10, 50, 2000),
                                                                             Teenager<int>(5, 13)
                                                                             );
    // time 1
    smalltown.tick(2); // monster and teenager dies

    auto status = smalltown.getStatus();

    assert( std::get<0>(status) != "");
    assert( std::get<1>(status) == 0);
    assert( std::get<2>(status) == 1);
    
    // time 3
    smalltown.tick(2); // nothing happens

    status = smalltown.getStatus();

    assert( std::get<0>(status) != "");
    assert( std::get<1>(status) == 0);
    assert( std::get<2>(status) == 1);

    smalltown.tick(3); // nothing happens

    status = smalltown.getStatus();

    assert( std::get<0>(status) != "");
    assert( std::get<1>(status) == 0);
    assert( std::get<2>(status) == 1);
}

int main() {
    no_fibonacci_ticking();
    monster_dies_after_attack();
    
    return 0;
}
