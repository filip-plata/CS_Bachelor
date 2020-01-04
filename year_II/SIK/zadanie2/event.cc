#include <cstring>
#include <cstdio>
#include <cstdint>
#include <iostream>
#include <sstream>

#include <boost/crc.hpp>  // for boost::crc_32_type

extern "C" {
#include "crc.h"
}

#include "utils.h"
#include "event.h"

using namespace std;


Event::Event() : event_no(0) {};


Event::Event(std::shared_ptr<EventData> edata, uint32_t event_no)
  : event_no(event_no), type(edata->event_type()),
    data(edata) {
    len = calc_len();
}


std::size_t Event::serialize(char *buf) {
    uint32_t crc32(0);
    std::size_t size(0);
    uint8_t type = data->event_type();
    len = calc_len();

    copy_to_raw_bytes(&len, buf + size, &size);
    copy_to_raw_bytes(&event_no, buf + size, &size);
    copy_to_raw_bytes(&type, buf + size, &size);
    size += data->serialize(buf + size);

    crc32 = crc32buf(buf, size);
    copy_to_raw_bytes(&crc32, buf + size, &size);

    return size;
}

uint32_t Event::calc_len() const {
    return 5 + data->calc_len();
}

uint32_t Event::absolute_size() const {
    return 12 + calc_len();
}

void Event::load(char *buf, std::size_t dlen) {
    std::size_t size(0),size_d(0);
    uint32_t crc32;
    boost::crc_32_type  result;

    if (dlen < 13)
        throw InvalidLoad("Message too short");

    copy_from_raw_bytes(&len, buf + size, &size);
    if (len > dlen - 8)
        throw InvalidLoad("Message length too big");
    copy_from_raw_bytes(&event_no, buf + size, &size);
    copy_from_raw_bytes(&type, buf + size, &size);

    result.process_bytes(buf, len + 4);

    copy_from_raw_bytes(&crc32, buf + len + 4, &size_d);

    if (crc32 != result.checksum()) {
        throw InvalidCrc();
    }

    switch (type) {
        case 0:
            data = std::make_unique<NewGame>();
            break;
        case 1:
            data = std::make_unique<Pixel>();
            break;
        case 2:
            data = std::make_unique<PlayerEliminated>();
            break;
        case 3:
            data = std::make_unique<GameOver>();
            break;
        default:
            throw InvalidType();
    }

    data->load(buf + size, (size_t) (len - 5));
}

void Event::load(char *buf) {
    load(buf, 0);
}

uint32_t Event::get_event_no() const {
    return event_no;
}

NewGame::NewGame() {}

NewGame::NewGame(uint32_t maxx, uint32_t maxy, vector<string> &pl)
  : maxx(maxx), maxy(maxy), players(pl) {}

uint8_t NewGame::event_type() const {
    return 0;
}

uint32_t NewGame::calc_len() const {
    uint32_t res(8);
    for (auto &pl : players)
        res += (pl.length() + 1);
    return res;
}

void NewGame::enrich_player_name(std::vector<std::string> &data) {
    data = players;
}

std::size_t NewGame::serialize(char *buf) {
    std::size_t size(0);

    copy_to_raw_bytes(&maxx, buf + size, &size);
    copy_to_raw_bytes(&maxy, buf + size, &size);

    for (auto &name : players) {
        std::strcpy(buf + size, name.c_str());
        size += (name.length() + 1);
    }

    return size;
}

std::size_t NewGame::serialize_text(char *buf) {
    std::ostringstream oss;

    oss << "NEW_GAME " << maxx << " " << maxy;

    for (std::string &pl : players) {
        oss << " " << pl;
    }

    oss << "\n";

    std::string code = oss.str();
    std::copy(code.begin(), code.end(), buf);
    buf[code.size()] = '\0';

    return code.size();
}

void NewGame::load(char *buf, std::size_t dlen) {
    std::size_t size(0);
    int len = dlen;

    copy_from_raw_bytes(&maxx, buf + size, &size);
    copy_from_raw_bytes(&maxy, buf + size, &size);

    len -= size;

    while (len > 0) {
        if (*(buf + size) == 0x00)
            throw InvalidLoad("Invalid New Game format");

        std::string pl(buf + size);
        if (invalid_player_name(pl))
            throw InvalidLoad("Invalid player name: " + pl);
        players.push_back(pl);

        len -= (pl.size() + 1);
        size += (pl.size() + 1);
    }
}

std::shared_ptr<NewGame> NewGame::create(
        uint32_t maxx, uint32_t maxy,
        std::vector<std::string> &pl_n) {
    return std::make_shared<NewGame>(
                maxx, maxy, pl_n);
}

Pixel::Pixel() {}

Pixel::Pixel(string pl, uint8_t pl_n, uint32_t x, uint32_t y)
  : player_name(pl), player_number(pl_n), x(x), y(y) {}

std::shared_ptr<Pixel> Pixel::create(std::string pl, uint8_t t_d,
              uint32_t x, uint32_t y) {
    return std::make_shared<Pixel>(
              pl, t_d,  x, y);
}

uint8_t Pixel::event_type() const {
    return 1;
}

uint32_t Pixel::calc_len() const {
    return 9;
}

void Pixel::enrich_player_name(std::vector<std::string> &data) {
    std::cout << "Data size " << data.size() << std::endl;
    std::cout << "Data size " << (int)player_number << std::endl;
    player_name = data.at(player_number);
}

std::size_t Pixel::serialize(char *buf) {
    std::size_t size(0);

    copy_to_raw_bytes(&player_number, buf + size, &size);
    copy_to_raw_bytes(&x, buf + size, &size);
    copy_to_raw_bytes(&y, buf + size, &size);

    return size;
}

void Pixel::load(char *buf, std::size_t dlen) {
    std::size_t size(0);

    copy_from_raw_bytes(&player_number, buf + size, &size);
    copy_from_raw_bytes(&x, buf + size, &size);
    copy_from_raw_bytes(&y, buf + size, &size);

    if (dlen != size)
        throw InvalidLoad("Pixel load invalid length");
}

std::size_t Pixel::serialize_text(char *buf) {
    std::size_t size(0);

    size = sprintf(buf, "%s %lu %lu %s\n", "PIXEL", (unsigned long)x,
            (unsigned long)y, player_name.c_str());

    return size;
}

PlayerEliminated::PlayerEliminated() {}

PlayerEliminated::PlayerEliminated(string pl, uint8_t p_n)
  : player_name(pl), player_number(p_n) {}

std::shared_ptr<PlayerEliminated> PlayerEliminated::create(
        string pl, uint8_t p_n) {
    return std::make_shared<PlayerEliminated>(
                    pl, p_n);
}

uint8_t PlayerEliminated::event_type() const {
    return 2;
}

uint32_t PlayerEliminated::calc_len() const {
    return 1;
}

void PlayerEliminated::enrich_player_name(std::vector<std::string> &data) {
    player_name = data.at(player_number);
}

std::size_t PlayerEliminated::serialize(char *buf) {
    std::size_t size(0);

    copy_to_raw_bytes(&player_number, buf + size, &size);

    return size;
}

void PlayerEliminated::load(char *buf, std::size_t dlen) {
    std::size_t size(0);

    copy_from_raw_bytes(&player_number, buf + size, &size);

    if (dlen != size)
        throw InvalidLoad("Player eliminated invalid length");
}

std::size_t PlayerEliminated::serialize_text(char *buf) {
    return sprintf(buf, "%s %s\n", "PLAYER_ELIMINATED",
              player_name.c_str());
}

GameOver::GameOver() {}

std::shared_ptr<GameOver> GameOver::create() {
    return std::make_shared<GameOver>();
}

uint8_t GameOver::event_type() const {
    return 3;
}

void GameOver::enrich_player_name(std::vector<std::string> &data) {
}

std::size_t GameOver::serialize(char *buf) {
    return 0;
}

void GameOver::load(char *buf, std::size_t dlen) {
   if (dlen)
      throw InvalidLoad("Game over needs no data to load");
}

uint32_t GameOver::calc_len() const {
    return 0;
}

std::size_t GameOver::serialize_text(char *buf) {
    return sprintf(buf, "%s\n", "GAME_OVER");
}
