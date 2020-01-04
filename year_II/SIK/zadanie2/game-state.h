#ifndef GAME_STATE_H
#define GAME_STATE_H

#include <vector>
#include <utility>
#include <memory>
#include <map>
#include <set>

#include <cstdint>

#include "communicate.h"
#include "event.h"


typedef std::pair<int64_t, int64_t> Field;

Field createField(int64_t, int64_t);


class Player {
public:
    Player();
    Player(std::string);
    const bool operator<(const Player&) const;

    std::string player_name;
    uint8_t player_number;
    int8_t turn_direction;
    long double snake_position_x;
    long double snake_position_y;;
    double snake_direction;
    bool ready;
    bool plays;
    bool disconnected;

    int64_t pos_x() const;
    int64_t pos_y() const;
    Field field() const;

    void move();
};

#endif /* GAME_STATE_H */
