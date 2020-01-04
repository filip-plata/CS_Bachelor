#ifndef EVENT_H
#define EVENT_H

#include <memory>
#include <cstdint>
#include <vector>
#include <string>
#include <stdexcept>

#include "serialize.h"
#include "serialize_text.h"

enum class EventID {NEW_GAME, PIXEL, PLAYER_ELIMINATED,
        GAME_OVER};

class InvalidCrc : public std::exception {};
class InvalidType : public std::exception {};


class EventData : public Serializable, public TextSerializable {
public:
    virtual uint8_t event_type() const = 0;
    virtual uint32_t calc_len() const = 0;
    /* Client needs to convert player numbers
      into player names somewhere */
    virtual void enrich_player_name(std::vector<std::string>&) = 0;
};

class Event : public Serializable {
public:
    Event();
    Event(std::shared_ptr<EventData>, uint32_t event_no);
    /* Must serialize event_type */
    std::size_t serialize(char *buf);
    void load(char*, std::size_t);
    void load(char*);
    uint32_t get_event_no() const;
    uint32_t calc_len() const;
    uint32_t absolute_size() const;

    uint32_t len;
    uint32_t event_no;
    uint8_t type;
    std::shared_ptr<EventData> data;
};

class NewGame : public EventData {
public:
    NewGame();
    NewGame(uint32_t, uint32_t, std::vector<std::string>&);

    void enrich_player_name(std::vector<std::string>&);

    static std::shared_ptr<NewGame> create(
            uint32_t maxx, uint32_t maxy,
            std::vector<std::string>&);

    uint8_t event_type() const;
    std::size_t serialize(char *buf);
    void load(char *, std::size_t);
    std::size_t serialize_text(char*);
    uint32_t calc_len() const;

    uint32_t maxx;
    uint32_t maxy;
    std::vector<std::string> players;
};

class Pixel : public EventData {
public:
    Pixel();
    Pixel(std::string, uint8_t, uint32_t, uint32_t);

    void enrich_player_name(std::vector<std::string>&);

    static std::shared_ptr<Pixel> create(
            std::string, uint8_t,
            uint32_t, uint32_t);

    uint8_t event_type() const;
    std::size_t serialize(char *buf);
    void load(char *, std::size_t);
    std::size_t serialize_text(char*);
    uint32_t calc_len() const;

    std::string player_name;
    uint8_t player_number;
    uint32_t x;
    uint32_t y;
};

class PlayerEliminated : public EventData {
public:
    PlayerEliminated();
    PlayerEliminated(std::string, uint8_t);

    void enrich_player_name(std::vector<std::string>&);

    static std::shared_ptr<PlayerEliminated> create(
        std::string, uint8_t
    );

    uint8_t event_type() const;
    std::size_t serialize(char *buf);
    void load(char *, std::size_t);
    std::size_t serialize_text(char*);
    uint32_t calc_len() const;

    std::string player_name;
    uint8_t player_number;
};

class GameOver : public EventData {
public:
    GameOver();
    static std::shared_ptr<GameOver> create();

    void enrich_player_name(std::vector<std::string>&);

    uint8_t event_type() const;
    std::size_t serialize(char *buf);
    void load(char *, std::size_t);
    std::size_t serialize_text(char*);
    uint32_t calc_len() const;
};

#endif /* EVENT_H */
