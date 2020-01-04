#include "../citizen.h"
#include "../monster.h"
#include "../smalltown.h"
#include <iostream>

using namespace std;

int main() {
    auto smallTown = SmallTown<Mummy<float>, int, 1, 23, Sheriff<int>, Adult<int>, Teenager<long>>(
            Mummy<float>(100.0f, 1.5f),
            Sheriff<int>(100, 35, 20),
            Adult<int>(100, 21),
            Teenager<long>(50, 14)
    );

    smallTown.tick(1);
    smallTown.tick(22);

    auto status = smallTown.getStatus();
    assert(get<0>(status) != "");
    assert(get<1>(status) == 60);
    assert(get<2>(status) == 3);

    return 0;
}
