#include <cmath>

#include "game-state.h"

#define PI 3.14159265

Field createField(int64_t x, int64_t y) {
    return std::make_pair(x, y);
}


Player::Player(std::string nick)
    : player_name(nick), turn_direction(0),
      snake_position_x(0.), snake_position_y(0.),
      snake_direction(0.), ready(false), plays(false),
      disconnected(false) {
}

Player::Player()
    : Player("") {}

int64_t Player::pos_x() const {
    return (int64_t) floor(snake_position_x);
}

int64_t Player::pos_y() const {
    return (int64_t) floor(snake_position_y);
}

void Player::move() {
    snake_position_x += cos(snake_direction * PI/180);
    snake_position_y += sin(snake_direction * PI/180);
}

Field Player::field() const {
    return createField(pos_x(), pos_y());
}

const bool Player::operator<(const Player &p) const {
    if (this->player_name != p.player_name) {
        return this->player_name < p.player_name;
    }

    return this->player_number < p.player_number;
}
