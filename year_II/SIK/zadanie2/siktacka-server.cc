#include <system_error>
#include <algorithm>
#include <memory>
#include <iostream>
#include <cstring>
#include <cassert>
#include <thread>

#include <poll.h>
#include <errno.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>

#include "generator.h"
#include "utils.h"
#include "siktacka-server.h"
#include "debug.h"

using namespace std::chrono;

enum State { GAME, NO_GAME };
typedef enum State State;


bool operator< (const struct sockaddr_in6 a,
                const struct sockaddr_in6 b) {
    if (b.sin6_addr.s6_addr < a.sin6_addr.s6_addr)
        return true;
    if (b.sin6_addr.s6_addr > a.sin6_addr.s6_addr)
        return false;
    return b.sin6_port < a.sin6_port;
}


bool operator== (const struct sockaddr_in6 a,
                 const struct sockaddr_in6 b) {
    if (b.sin6_addr.s6_addr == a.sin6_addr.s6_addr)
        return true;
    if (b.sin6_port == a.sin6_port)
        return true;
    return false;
}


bool operator< (const struct sockaddr_in a,
                const struct sockaddr_in b) {
    if (b.sin_addr.s_addr < a.sin_addr.s_addr)
        return true;
    if (b.sin_addr.s_addr > a.sin_addr.s_addr)
        return false;
    return b.sin_port < a.sin_port;
}


Server::Server(config *conf)
    : conf(conf), game(this, conf->WIDTH, conf->HEIGHT) {
    int err = 0;
    int master_socket;

    set_seed(conf->SEED);

    if ((master_socket = socket(PF_INET6, SOCK_DGRAM, 0)) == -1)
        throw std::system_error();

    memset(&server, '\0', sizeof(server));
    server.sin6_family = PF_INET6;
    server.sin6_port = htons(conf->PORT);
    server.sin6_addr = in6addr_any;

    /* Support for IPV_4 */

    if ((err = setsockopt(master_socket, IPPROTO_IPV6,
                          IPV6_V6ONLY, (void *)&err, sizeof(err))))
        throw std::system_error();

    if (fcntl(master_socket, F_SETFL, O_NONBLOCK) < 0)
        throw std::system_error(EFAULT, std::system_category());

    if (bind(master_socket, (struct sockaddr*)&server, sizeof(server)) == -1)
        throw std::system_error();

    client_msg = std::make_unique<ServerFromClientMessage>(
          master_socket
    );
}


void Server::run() {
    int activity(0);
    int timeout(-1);
    microseconds turn(1000000 / conf->ROUNDS_PER_SEC);
    microseconds time_left(turn), before_poll;
    struct pollfd fd;
    fd.fd = client_msg->get_socket();
    fd.events = POLLIN;

    while (true) {
        refresh_clients();
        if (game)
            timeout = std::max((long) time_left.count()/1000, 0L);
        else
            timeout = -1;

        before_poll = time_now();
        DEBUG("Server timeout ") << timeout << std::endl;
        activity = poll(&fd, 1, timeout);

        if (game)
          time_left -= (time_now() - before_poll);

        DEBUG("Server next turn ") << time_left.count() << std::endl;

        if (time_left.count() <= 0) {
            game.tick();
            time_left = turn;
        }


        if (activity < 0)
            continue;

        if (activity) {
            handle_incoming_data();
        }
    }
}

void Server::refresh_clients(
    microseconds timeout) {
    microseconds curr_time = time_now();
    std::vector<ConnInfo> erase;

    for (auto it : client_times) {
        if (curr_time - it.second > timeout) {
            erase.push_back(it.first);
        }
    }

    for (auto &conn_info : erase) {
        remove_client(conn_info);
    }
}

void Server::send_reply(uint32_t last_event, ConnInfo conn) {
    DEBUG("Sending reply to client");
    auto server_msg = server_msgs[conn];
    try {
        server_msg->send_message(last_event);
    } catch (WouldBlock &e) {
        std::cerr << "UDP kernel buffer is full??" << std::endl;
    } catch(NetworkUnreachable &e) {
        std::cerr << "Network problems" << std::endl;
        std::this_thread::sleep_for(seconds(1));
    }
}

void Server::send_replies(uint32_t event_no) {
    try {
        for (auto &info : server_msgs)
            info.second->send_message(event_no);
    } catch (WouldBlock &e) {
        std::cerr << "UDP kernel buffer is full??" << std::endl;
    } catch (NetworkUnreachable &e) {
        std::cerr << "Network problems" << std::endl;
        std::this_thread::sleep_for(seconds(1));
    }
}


bool Server::player_name_used(std::string &pl_name) {
    auto nicks = game.player_names();
    return std::find(nicks.begin(), nicks.end(), pl_name) != nicks.end();
}


void Server::handle_incoming_data() {
    try {
        client_msg->receive_message();
    } catch (InvalidLoad &e) {
        return;
    }
    ConnInfo conn_info = std::make_pair(client_msg->info,
                client_msg->session_id);


    auto it = std::find_if(server_msgs.begin(), server_msgs.end(),
    [&conn_info](auto const &msg) {
        return msg.first.first == conn_info.first;
    });

    if (it != server_msgs.end()) {
        DEBUG("Previous client detected");
        if (client_msg->session_id > it->first.second) {
            DEBUG("Switching clients");
            remove_client(conn_info);
            handle_new_client(conn_info);
        }
    } else {
        std::string &pl_name = client_msg->player_name;
        if (server_msgs.size() >= ConnLimit) {
            std::cerr << "Too many clients" << std::endl;
            return;
        }
        if (player_name_used(pl_name)) {
            std::cerr << "Player name in use " << pl_name << std::endl;
            return;
        }
        handle_new_client(conn_info);
    }

    client_times[conn_info] = time_now();

    game.set_turn_direction(conn_info,
                            client_msg->turn_direction);

    game.try_start();

    send_reply(client_msg->next_expected_event_no,
                conn_info);
}

void Server::handle_new_client(ConnInfo conn_info) {
    auto it = clients.insert(conn_info);
    auto ptr = std::make_shared<struct sockaddr*>((struct sockaddr*)&(it.first->first));
    server_msgs[conn_info] = std::make_unique<ServerToClientMessage>(
      client_msg->get_socket(), ptr
    );
    char str[200];

    inet_ntop(PF_INET6, &(conn_info.first.sin6_addr), str, 200);
    std::cout << "Server msger: " << str << std::endl;
    server_msgs[conn_info]->events_server = &events;
    server_msgs[conn_info]->game_id = game.game_id;
    game.add_player(conn_info, client_msg->player_name);
}

void Server::remove_client(ConnInfo &conn_info) {
    server_msgs.erase(conn_info);
    client_times.erase(conn_info);
    clients.erase(conn_info);

    if (game) {
        game.player_disconnected(conn_info);
    } else {
        game.players.erase(conn_info);
    }
}

Server::GameState::GameState(Server *s, uint32_t maxx, uint32_t maxy)
    : started(false), maxx(maxx), maxy(maxy),
      last_event_no_(0), server(s)  {}

void Server::GameState::player_disconnected(ConnInfo &conn_info) {
    players[conn_info].disconnected = true;
}

void Server::GameState::add_player(ConnInfo &con, std::string nick) {
    players[con] = Player(nick);
}

Server::GameState::operator bool() const {
    return started;
}

void Server::GameState::tick() {
    DEBUG("Game tick") << std::endl;
    for (auto &p : players) {
        if (!p.second.plays)
            continue;
        Field prev = p.second.field();
        p.second.snake_direction += p.second.turn_direction *
            server->conf->TURNING_SPEED;
        p.second.move();
        if (p.second.field() == prev)
            continue;

        helper_check_player_position(p);
    }

    try_end();
}

void Server::GameState::helper_check_player_position(auto &it) {
    if(check_player_position(it.second)) {
        it.second.plays = false;
        auto player_eliminated_data = PlayerEliminated::create(
              it.second.player_name, it.second.player_number
        );
        player_eliminated_event(player_eliminated_data);
    } else {
        auto pixel_data = Pixel::create(it.second.player_name,
                        it.second.player_number,
                it.second.pos_x(), it.second.pos_y());
        pixel_event(pixel_data);
    }
}

bool Server::GameState::check_player_position(Player const &p) {
    Field pos = createField(p.pos_x(), p.pos_y());
    const bool is_taken = taken_fields.find(pos)
        != taken_fields.end();

    if (out_of_range(pos)) {
        DEBUG("Player out of range pos_x ") << pos.first
        << " pos_y " << pos.second;
    }

    if (is_taken) {
        DEBUG("Field is taken pox_x ") << pos.first
        << " pos_y " << pos.second;
    }

    return is_taken || out_of_range(pos);
}

bool Server::GameState::try_end() {
    int playing = std::count_if(players.begin(), players.end(),
                             [](auto const &p) {
                                 return p.second.plays;
                             });
    if (playing <= 1) {
        end_game();
    }

    return started;
}

bool Server::GameState::out_of_range(Field &f) {
    return f.first < 0 || f.second < 0 || f.first > maxx || f.second > maxy;
}

bool Server::GameState::try_start() {
    if (started)
        return true;

    bool can = std::any_of(players.begin(),
                           players.end(), [](auto const &p) {
                               return (p.second.player_name != "" &&
                                       !p.second.ready);
                           });
    int players_c = std::count_if(players.begin(), players.end(),
                  [](auto const &p) {
                    return p.second.player_name != "";
                });

    DEBUG("Players count: ") << players_c << std::endl;

    if (!can && players_c > 1) {
        ensure_game_event_size();
        start_game();
    }

    return started;
}


void Server::GameState::ensure_game_event_size() {
  auto pl_n = player_names();
  auto event_data = NewGame::create(maxx, maxy, pl_n);
  auto event = Event(event_data, 0);

  while (event.absolute_size() > ServerToClientMessage::MAX_MSG_SIZE) {
      auto it(server->clients.begin());
      std::advance(it, rand() % server->clients.size());
      ConnInfo inf = *it;
      server->remove_client(inf);
      pl_n = player_names();
      event_data = NewGame::create(maxx, maxy, pl_n);
      event = Event(event_data, 0);
  }
}


void Server::GameState::pixel_event(std::shared_ptr<Pixel> data) {
    taken_fields.insert(createField(data->x, data->y));
    event_generated(data);
}

void Server::GameState::player_eliminated_event(
      std::shared_ptr<PlayerEliminated> data) {
      event_generated(data);
}

void Server::GameState::event_generated(std::shared_ptr<EventData> e) {
    event_generated(Event(e, last_event_no()));
}

void Server::GameState::event_generated(Event e) {
    DEBUG("Generated event with type: ") << (int)e.type
    << " and number " << e.get_event_no() << std::endl;
    server->events.push_back(e);
    server->send_replies(e.get_event_no());
}

std::vector<std::string> Server::GameState::player_names() const {
    std::vector<std::string> res;

    for (auto &p : players)
        if (p.second.player_name != "")
            res.push_back(p.second.player_name);

    return res;
}

void Server::GameState::game_start_event(std::shared_ptr<NewGame> d) {
    uint8_t player_numbers = 0;
    game_id = next();
    taken_fields.clear();
    server->events.clear();
    last_event_no_ = 0;

    for (auto &p : players) {
        if (p.second.player_name != "") {
            p.second.plays = true;
            p.second.player_number = player_numbers++;
        }
    }
    started = true;
    for (auto &s_msg : server->server_msgs) {
        s_msg.second->game_id = game_id;
    }
    event_generated(d);
}

void Server::GameState::start_game() {
    auto pl_n = player_names();
    game_start_event(NewGame::create(maxx, maxy, pl_n));

    for (auto &p : players) {
        if (!p.second.plays)
            continue;
        p.second.snake_position_x = (next() % maxx) + 0.5;
        p.second.snake_position_y = (next() % maxy) + 0.5;
        p.second.snake_direction = (next() % 360);

        auto &pl = p.second;

        DEBUG("Player ") << pl.player_name << " pos_x " << pl.pos_x()
           << " pos_y " << pl.pos_y() << std::endl;

        helper_check_player_position(p);
    }
}

void Server::GameState::game_over_event(
    std::shared_ptr<GameOver> end_game_data) {
    for (auto it = players.begin(); it != players.end();) {
        if (it->second.disconnected) {
            it = players.erase(it);
        } else {
            ++it;
        }
    }
    for (auto &p : players) {
        p.second.plays = false;
        p.second.ready = false;
        p.second.turn_direction = 0;
        p.second.snake_direction = 0.0;
    }
    started = false;
    event_generated(end_game_data);
}

void Server::GameState::end_game() {
    game_over_event(GameOver::create());
    DEBUG("GAME OVER");
}

void Server::GameState::set_turn_direction(ConnInfo &conn_in, int8_t t_d) {
    Player &pl = players[conn_in];

    if (pl.turn_direction != t_d) {
        DEBUG("Changing turn direction");
    }

    pl.turn_direction = t_d;

    if (t_d != 0 && !pl.ready && pl.player_name != "") {
        pl.ready = true;
        DEBUG("Player is now ready: ") << pl.player_name << std::endl;
    }
}

uint32_t Server::GameState::last_event_no() {
    return last_event_no_++;
}
