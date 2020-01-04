#include "../smalltown.h"
#include <iostream>
#include <limits>

int main() {
    auto smalltown = SmallTown<Zombie<float>, long long, std::numeric_limits<int>::max() - 300, std::numeric_limits<int>::max() - 30, Teenager<int>>(
                                                                               Zombie<float>(20, 1),
                                                                           Teenager<int>(15, 15)
                                                                           );
    smalltown.tick(30);
    
    return 0;
}
