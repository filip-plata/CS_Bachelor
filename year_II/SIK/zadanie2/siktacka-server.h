#ifndef SIKTACKA_SERVER_H
#define SIKTACKA_SERVER_H

#include <map>
#include <utility>
#include <memory>
#include <chrono>
#include <queue>
#include <cstdint>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>

#include "event.h"
#include "game-state.h"
#include "config.h"
#include "communicate.h"


class Server {
public:
    typedef struct sockaddr_in6 addr_in;
    typedef std::pair<addr_in, uint64_t> ConnInfo;

    Server(config *conf);

    void run();
private:
    static const std::size_t ConnLimit = 42;

    config *conf;
    struct sockaddr_in6 server;
    std::unique_ptr<ServerFromClientMessage> client_msg;
    std::map<ConnInfo, std::shared_ptr<ServerToClientMessage>> server_msgs;
    std::map<ConnInfo, std::chrono::microseconds> client_times;
    std::set<ConnInfo> clients;

    void handle_incoming_data();
    void send_reply(uint32_t, ConnInfo);
    void refresh_clients(std::chrono::microseconds
              = std::chrono::microseconds(2000000));
    std::vector<Event> events;

    void handle_new_client(ConnInfo);
    void remove_client(ConnInfo&);
    void send_replies(uint32_t);
    bool player_name_used(std::string &pl_name);

    class GameState {
    public:
        uint32_t game_id;
        std::map<ConnInfo, Player> players;

        GameState(Server*, uint32_t maxx, uint32_t maxy);
        void handle_message(ServerFromClientMessage&);
        void tick();
        operator bool() const;
        void add_player(ConnInfo&, std::string);
        void player_disconnected(ConnInfo&);
        bool try_start();
        void set_turn_direction(ConnInfo&, int8_t);
        std::vector<std::string> player_names() const;
    private:
        bool started;
        uint32_t maxx;
        uint32_t maxy;
        uint32_t last_event_no_;
        std::set<Field> taken_fields;
        Server *server;

        GameState();
        void start_game();
        void end_game();
        bool try_end();
        void event_generated(Event);
        void event_generated(std::shared_ptr<EventData>);
        bool out_of_range(Field&);
        uint32_t last_event_no();
        void player_eliminated_event(std::shared_ptr<PlayerEliminated>);
        void pixel_event(std::shared_ptr<Pixel>);
        void game_over_event(std::shared_ptr<GameOver>);
        void game_start_event(std::shared_ptr<NewGame>);
        bool check_player_position(Player const &);
        void helper_check_player_position(auto&);
        void ensure_game_event_size();
    };

    GameState game;
};

#endif /* SIKTACKA_SERVER_H */
