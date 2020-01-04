#include "utils.h"
#include <iostream>

using namespace std::chrono;

bool invalid_player_name(std::string &player_name) {
    bool result = true;
    result &= player_name.length() < 65;

    result &= std::all_of(player_name.begin(), player_name.end(), [](char c) {
                auto ascii = (int) c;
                return (33 <= ascii) && (ascii <= 126);
            });

    return !result;
}

bool invalid_turn_direction(int8_t val) {
    return ((int)val != -1) && ((int)val != 0) && ((int)val != 1);
}

microseconds time_now() {
    return duration_cast<microseconds>(
        system_clock::now().time_since_epoch()
    );
}
