#ifndef SIKTACKA_CLIENT_H
#define SIKTACKA_CLIENT_H

#include <memory>
#include <queue>
#include <set>

#include <cstdint>

#include "communicate.h"
#include "config.h"

struct Compare: std::binary_function<Event, Event, bool>
{
    bool operator()(const Event& lhs, const Event& rhs) const {
        return lhs.event_no > rhs.event_no;
    }
};


class Client {
public:
    Client(config_client*);
    ~Client() { close(socket_ui); close(socket_server); };

    void run();
private:
    config_client *conf;
    int socket_server;
    int socket_ui;
    uint32_t game_id;
    uint32_t last_event;
    std::priority_queue<Event, std::vector<Event>, Compare> game_events;
    std::set<uint32_t> events_no;

    std::unique_ptr<ServerFromClientMessage> client_msg;
    std::unique_ptr<ServerToClientMessage> server_msg;
    std::unique_ptr<UiToClientMessage> ui_msg;
    std::vector<std::string> players;

    void send_request();
    void handle_incoming_data_server();
    void handle_incoming_data_ui();
    void gui_communicate();
    bool is_gui_data() const;
    bool has_event(uint32_t) const;
};


#endif /* SIKTACKA_CLIENT_H */
