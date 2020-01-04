#include <chrono>
#include <iostream>
#include <system_error>

#include <poll.h>
#include <errno.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "config.h"
#include "utils.h"
#include "siktacka-client.h"
#include "debug.h"

using namespace std::chrono;

enum state {SEND_TO_SERVER, SEND_TO_GUI, RECEIVE};
typedef enum state state;


Client::Client(config_client *conf)
  :  last_event(0), events_no()
   {
     int err = 0, flag = 0;
     struct sockaddr_in6 server;

     server.sin6_family = PF_INET6;
     server.sin6_port = 0;
     server.sin6_addr= in6addr_any;

     if ((socket_server = socket(PF_INET6, SOCK_DGRAM, IPPROTO_UDP)) == -1)
         throw std::system_error(EFAULT, std::system_category());
 #ifdef ONLYIP4
     if (fcntl(socket_server, F_SETFL, O_NONBLOCK) < 0)
         throw std::system_error(EFAULT, std::system_category());
 #endif
     /* Support for IPV_4 */

     if ((err = setsockopt(socket_server, IPPROTO_IPV6,
                           IPV6_V6ONLY, (void *)&flag, sizeof(flag))))
         throw std::system_error(EFAULT, std::system_category());

     if (bind(socket_server, (struct sockaddr*) &server,
         sizeof(server)) == -1)
         throw std::system_error(EFAULT, std::system_category());

    int ui_prot = conf->ui_server->sa_family;
    if ((socket_ui = socket(ui_prot, SOCK_STREAM, 0)) == -1)
        throw std::system_error(EFAULT, std::system_category());

    /* Support for IPV_4 */

    if (ui_prot == PF_INET6)
        if ((err = setsockopt(socket_ui, IPPROTO_IPV6,
                              IPV6_V6ONLY, (void *)&flag, sizeof(flag))))
            throw std::system_error(EFAULT, std::system_category());

    if ((err = setsockopt(socket_ui, IPPROTO_TCP, TCP_NODELAY,
                            (char *) &flag, sizeof(int)) == -1))
        throw std::system_error(EFAULT, std::system_category());


    connect(socket_ui, conf->ui_server, sizeof(*(conf->ui_server)));

    if (fcntl(socket_ui, F_SETFL, O_NONBLOCK) < 0)
        throw std::system_error(EFAULT, std::system_category());

    auto ptr_server = std::make_shared<struct sockaddr*>(conf->game_server);

    /* Initialize message managers */
    client_msg = std::make_unique<ServerFromClientMessage>(
            socket_server, conf->player_name,
            ptr_server);
    ui_msg = std::make_unique<UiToClientMessage>(
                socket_ui, std::make_shared<struct sockaddr*>(conf->ui_server));
    server_msg = std::make_unique<ServerToClientMessage>(
          socket_server, nullptr);
}

void Client::run() {
  int activity(0);
  int timeout(-1);
  microseconds next_send, turn(20000);
  microseconds turn_safety(3000);
  struct pollfd fd[2];
  fd[0].fd = socket_server;
  fd[1].fd = socket_ui;
  fd[0].events = POLLIN;
  fd[1].events = POLLIN;
  state st = SEND_TO_SERVER;

  next_send = time_now() + turn;

  while (true) {
      switch (st) {
          case SEND_TO_SERVER:
              try {
                  send_request();
              } catch (WouldBlock &e) {
              }
              st = SEND_TO_GUI;
              break;
          case SEND_TO_GUI:
              while (is_gui_data() &&
                    next_send - time_now() > turn_safety) {
                  try {
                      gui_communicate();
                  } catch (WouldBlock &e) {
                      break;
                  }
              }
              st = RECEIVE;
              break;
          case RECEIVE:
              timeout = std::max((long) (next_send - time_now()).count() / 1000, 0L);
              DEBUG("Client timeout: ") << timeout << std::endl;
              activity = poll(fd, 2, timeout);
              if (activity < 0)
                  continue;

              if (activity) {
                  if (fd[0].revents & POLLIN)
                      handle_incoming_data_server();
                  if (fd[1].revents & POLLIN)
                      handle_incoming_data_ui();
              } else {
                  st = SEND_TO_SERVER;
                  next_send += turn;
              }
              break;
      }
  }
}

void Client::send_request() {
    client_msg->turn_direction = ui_msg->turn_direction;
    client_msg->next_expected_event_no = last_event;

    DEBUG("Send request to server: next_e_no ") <<
        client_msg->next_expected_event_no << " turn_direction "
        << (int)client_msg->turn_direction << " session_id "
        << client_msg->session_id << std::endl;

    client_msg->send_message();
}

bool Client::is_gui_data() const {
    return !game_events.empty() &&
            game_events.top().get_event_no() < last_event;
}

void Client::handle_incoming_data_server() {
    server_msg->receive_message();
    DEBUG("Received from server: game_id: ") <<
          server_msg->game_id << std::endl;

    for (auto &e : server_msg->events) {
        DEBUG("Received from server: event_type: ") <<
              (int)e.type << " event_no " << e.event_no
              << std::endl;
        if (e.type == 0 && game_id != server_msg->game_id) {
            game_id = server_msg->game_id;
            events_no.clear();
            players.clear();
            game_events = std::priority_queue<Event, std::vector<Event>, Compare>();
            last_event = 0;
        }
        if (server_msg->game_id == game_id) {
            e.data->enrich_player_name(players);
            if (has_event(e.event_no))
                continue;
            if (e.type != 3) {
                DEBUG("Pushed event to GUI event queue");
                game_events.push(e);
            }
            events_no.insert(e.event_no);

            while (events_no.find(last_event) != events_no.end()) {
                last_event++;
            }
        }
    }
    server_msg->events.clear();
    DEBUG("Current last_event: ") << last_event << std::endl;
}

bool Client::has_event(uint32_t event_no) const {
    return events_no.find(event_no) != events_no.end();
}

void Client::handle_incoming_data_ui() {
    try {
        ui_msg->receive_message();
        DEBUG("Received from GUI: ") <<
              (int) ui_msg->turn_direction << std::endl;
    } catch (InvalidMessage &e) {
        std::cerr << e.what() << std::endl;
    }
}

void Client::gui_communicate() {
    auto event = game_events.top().data;
    ui_msg->event = event;
    DEBUG("Sended to GUI: ") << (int)event->event_type() << std::endl;

    ui_msg->send_message();
    game_events.pop();
    ui_msg->event = nullptr;
}
