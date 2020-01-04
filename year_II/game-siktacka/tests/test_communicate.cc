#include <iostream>
#include <memory>

#include <cassert>
#include <cstring>
#include <stdexcept>

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <unistd.h>

#include "../communicate.h"
#include "../event.h"
#include "../utils.h"
#include "test_event.h"

static const std::size_t BSIZE = 1000;
static char buf[BSIZE];

using namespace std;

void test_server_from_client_message_load_valid() {
    ServerFromClientMessage client_msg;
    std::size_t size(0);

    /* Session id */
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0xA2;
    buf[size++] = 0x0E;
    buf[size++] = 0x00;
    buf[size++] = 0x17;
    /* turn_direction */
    buf[size++] = 0x01;
    /* next_expected_event_no */
    buf[size++] = 0x00;
    buf[size++] = 0x01;
    buf[size++] = 0xA1;
    buf[size++] = 0x1F;
    /* player_name */
    buf[size++] = 0x00;
    size--;

    try {
        client_msg.load(buf, size);
        assert (true);
    } catch (InvalidLoad &e) {
        assert (false);
    }

    assert (client_msg.session_id == 2718826519);
    assert (client_msg.turn_direction == 1);
    assert (client_msg.next_expected_event_no == 106783);
    assert (client_msg.player_name == "");

    buf[8] = 0x00;
    size = 13;
    buf[size++] = 'M';
    buf[size++] = 'a';
    buf[size++] = 'r';
    buf[size++] = 's';
    buf[size++] = 'z';
    buf[size++] = 'a';
    buf[size++] = 'n';
    buf[size++] = 'd';

    buf[size++] = 0x00;
    size--;

    try {
        client_msg.load(buf, size);
        assert (true);
    } catch (InvalidLoad &e) {
        assert (false);
    }

    assert (client_msg.session_id == 2718826519);
    assert (client_msg.turn_direction == 0);
    assert (client_msg.next_expected_event_no == 106783);
    assert (client_msg.player_name == "Marszand");
}

void test_server_from_client_message_load_invalid() {
    ServerFromClientMessage client_msg;
    std::size_t size(0);

    /* Session id */
    buf[size++] = 0xA8;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0x00;
    buf[size++] = 0xA2;
    buf[size++] = 0x0E;
    buf[size++] = 0x00;
    buf[size++] = 0x17;
    /* turn_direction - INVALID */
    buf[size++] = 0x02;
    /* next_expected_event_no */
    buf[size++] = 0x00;
    buf[size++] = 0x01;
    buf[size++] = 0xA1;
    buf[size++] = 0x1F;
    /* player_name */
    buf[size++] = 0x00;

    try {
        client_msg.load(buf, size);
        assert (false);
    } catch (InvalidLoad &e) {
        assert (true);
    }

    buf[8] = -1;
    /* INVALID player_name */
    buf[size-1] = 'D';
    buf[size++] = 'A';
    buf[size++] = 'G';
    buf[size++] = 'O';
    buf[size++] = ' ';
    buf[size++] = 'N';
    buf[size++] = 0x00;

    try {
        client_msg.load(buf, size);
        assert (false);
    } catch (InvalidLoad &e) {
        assert (true);
    }
}

void test_server_from_client_message_serialize() {
    ServerFromClientMessage client_msg, copy;
    client_msg.session_id = 7568126430489LL;
    client_msg.next_expected_event_no = 3484541201L;
    client_msg.turn_direction = -1;
    client_msg.player_name = "Chlosta";

    assert (client_msg.serialize(buf) == 20);
    std::size_t size(0);
    try {
        copy.load(buf, 20);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert(copy.turn_direction == -1);
    assert(copy.next_expected_event_no == 3484541201L);
    assert(copy.session_id == 7568126430489LL);
    assert(copy.player_name == "Chlosta");

    /* Session id */
    assert (buf[size++] == (char) 0x00);
    assert (buf[size++] == (char) 0x00);
    assert (buf[size++] == (char) 0x06);
    assert (buf[size++] == (char) 0xE2);
    assert (buf[size++] == (char) 0x17);
    assert (buf[size++] == (char) 0x7C);
    assert (buf[size++] == (char) 0xCD);
    assert (buf[size++] == (char) 0x19);

    /* turn_direction */
    assert (buf[size++] == (char) (-1));
    /* next_expected_event_no */
    assert (buf[size++] == (char) 0xCF);
    assert (buf[size++] == (char) 0xB1);
    assert (buf[size++] == (char) 0xE1);
    assert (buf[size++] == (char) 0x11);
    /* player_name */
    assert (buf[size++] == 'C');
    assert (buf[size++] == 'h');
    assert (buf[size++] == 'l');
    assert (buf[size++] == 'o');
    assert (buf[size++] == 's');
    assert (buf[size++] == 't');
    assert (buf[size++] == 'a');
    assert (buf[size++] == 0x00);
}

void test_server_to_client_message_load_valid() {
    ServerToClientMessage server_msg;
    std::size_t size(0);
    Event game, pixel1, pixel2;
    NewGame g;
    Pixel p1, p2;

    g.maxx = 10784;
    g.maxy = 1024;
    g.players.push_back("xdd");

    p1.player_number = 1;
    p1.x = 1015;
    p1.y = 58;

    p2.player_number = 1;
    p2.x = 10500;
    p2.y = 548;

    game.data = std::make_unique<NewGame>(g);
    pixel1.data = std::make_unique<Pixel>(p1);
    pixel2.data = std::make_unique<Pixel>(p2);

    buf[size++] = 0x00;
    buf[size++] = 0x2E;
    buf[size++] = 0x00;
    buf[size++] = 0x80;

    size += game.serialize(buf + size);
    size += pixel1.serialize(buf + size);
    size += pixel2.serialize(buf + size);

    try {
        server_msg.load(buf, size);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (server_msg.game_id == 3014784);
    assert (server_msg.events.size() == 3);
    assert (server_msg.events[0].data->event_type() == 0);
    assert (server_msg.events[1].data->event_type() == 1);
    assert (server_msg.events[2].data->event_type() == 1);
}

void test_server_to_client_message_load_invalid_crc() {
    ServerToClientMessage server_msg;
    std::size_t size(0);
    Event game, player, game_over;
    NewGame g;
    PlayerEliminated pe;
    GameOver go;

    g.maxx = 10784;
    g.maxy = 1024;
    g.players.push_back("xdd");
    g.players.push_back("Roko");

    pe.player_number = 1;

    game.data = std::make_unique<NewGame>(g);
    player.data = std::make_unique<PlayerEliminated>(pe);
    game_over.data = std::make_unique<GameOver>(go);

    buf[size++] = 0x00;
    buf[size++] = 0x2E;
    buf[size++] = 0x00;
    buf[size++] = 0x80;

    size += game.serialize(buf + size);
    size += player.serialize(buf + size);
    size += game_over.serialize(buf + size);

    /* change player eliminated crc */
    buf[46] = 0x0E;

    try {
        server_msg.load(buf, size);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (server_msg.game_id == 3014784);
    assert (server_msg.events.size() == 1);
    assert (server_msg.events[0].data->event_type() == 0);
}

void test_server_to_client_message_load_invalid_random_junk() {
    ServerToClientMessage server_msg;
    std::size_t cases(10), size;

    for (std::size_t i=0; i<cases; i++) {
        size = rand() % 512;
        for(std::size_t j = 0; j < size; j++)
            buf[j] = rand() % 256;
        try {
            server_msg.load(buf, size);
            assert (false);
        } catch (InvalidLoad &e) {
            assert (true);
        }
    }
}

void test_server_to_client_message_load_invalid_type() {
    ServerToClientMessage server_msg;
    std::size_t size(0);
    Event game, player, game_over;
    NewGame g;
    PlayerEliminated pe;
    GameOver go;

    g.maxx = 10784;
    g.maxy = 1024;
    g.players.push_back("xdd");
    g.players.push_back("Roko");

    pe.player_number = 1;

    game.data = std::make_unique<NewGame>(g);
    player.data = std::make_unique<PlayerEliminated>(pe);
    game_over.data = std::make_unique<GameOver>(go);

    buf[size++] = 0x00;
    buf[size++] = 0x2E;
    buf[size++] = 0x00;
    buf[size++] = 0x80;

    size += game.serialize(buf + size);
    size += player.serialize(buf + size);
    size += game_over.serialize(buf + size);

    /* change new game type to junk */

    buf[12] = 0x04;

    buf[30] = 0x3D;
    buf[31] = 0xD9;
    buf[32] = 0x5A;
    buf[33] = 0xFD;

    try {
        server_msg.load(buf, size);
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }

    assert (server_msg.game_id == 3014784);
    assert (server_msg.events.size() == 2);
    assert (server_msg.events[0].data->event_type() == 2);
    assert (server_msg.events[1].data->event_type() == 3);
}

void test_server_to_client_message_load_too_long() {
    ServerToClientMessage server_msg;

    try {
        server_msg.load(buf, 513);
        assert (false);
    } catch (InvalidLoad &e) {
        assert (true);
    }
}

void test_ui_to_client_message_load() {
    const char *LEFT_K_D = "LEFT_KEY_DOWN\n";
    const char *LEFT_K_U = "LEFT_KEY_UP\n";
    const char *RIGHT_K_U = "RIGHT_KEY_UP\n";
    const char *RIGHT_K_D = "RIGHT_KEY_DOWN\n";
    const char *JUNK = "XDDDD FDF1";
    UiToClientMessage ui_msg;

    /* LEFT_KEY_DOWN */
    sprintf(buf, "%s", LEFT_K_D);
    try {
        ui_msg.load(buf, strlen(LEFT_K_D));
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }
    assert (ui_msg.turn_direction == -1);
    memset(buf, 0, sizeof(buf));

    /* LEFT_KEY_UP */
    sprintf(buf, "%s", LEFT_K_U);
    try {
      ui_msg.load(buf, strlen(LEFT_K_U));
    } catch (std::exception &e) {
        assert(false);
    }
    assert (ui_msg.turn_direction == 0);
    memset(buf, 0, sizeof(buf));

    /* RIGHT_KEY_UP */
    sprintf(buf, "%s", RIGHT_K_U);
    try {
        ui_msg.load(buf, strlen(RIGHT_K_U));
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }
    assert (ui_msg.turn_direction == 0);
    memset(buf, 0, sizeof(buf));

    /* RIGHT_KEY_DOWN */
    sprintf(buf, "%s", RIGHT_K_D);
    try {
        ui_msg.load(buf, strlen(RIGHT_K_D));
        assert (true);
    } catch (std::exception &e) {
        assert (false);
    }
    assert (ui_msg.turn_direction == 1);
    memset(buf, 0, sizeof(buf));

    /* JUNK */
    sprintf(buf, "%s", JUNK);
    try {
        ui_msg.load(buf, strlen(JUNK));
        assert(false);
    } catch (std::exception &e) {
        assert(true);
    }
    memset(buf, 0, sizeof(buf));
}

void test_server_to_client_message_send_message_simple() {
    int sock_server, sock_client, game_id = 4684;
    int port_server = 54002, port_client = 54003;
    struct sockaddr_in server, client;

    Pixel px1, px2;
    NewGame game;

    game.maxx = 545846;
    game.maxy = 4648;
    game.players = vector<string>({"ps", "pa", "xd", "lol"});

    px1.player_number = 1;
    px1.x = 466;
    px1.y = 466;

    px2.player_number = 7;
    px2.x = 46658;
    px2.y = 4646;

    Event e1, e2, e3;
    e1.event_no = 2;
    e1.type = 1;
    e1.data = std::make_shared<Pixel>(px1);

    e2.event_no = 3;
    e2.type = 1;
    e2.data = std::make_shared<Pixel>(px2);

    e3.event_no = 1;
    e3.type = 0;
    e3.data = std::make_shared<NewGame>(game);

    vector<Event> events_all({
                  e1,
                  e2,
                  e3,
        });
    vector<Event> events_first = events_all;

    if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        assert (false);

    if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        assert (false);

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_port = htons(port_server);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    memset((char *) &client, 0, sizeof(client));
    client.sin_family = PF_INET;
    client.sin_port = htons(port_client);

    if (inet_aton("127.0.0.1" , &client.sin_addr) == 0)
        assert (false);

    if (bind(sock_server, (struct sockaddr*)&server, sizeof(server)) == -1)
        assert (false);

    if (bind(sock_client, (struct sockaddr*)&client, sizeof(client)) == -1)
        assert (false);

    auto ptr = std::make_shared<struct sockaddr*>((struct sockaddr*)&client);

    ServerToClientMessage server_msg(sock_server, ptr);
    ServerToClientMessage server_msg_rec(sock_client, nullptr);

    server_msg.game_id = game_id;
    server_msg.events = events_all;

    server_msg.send_message(0);
    server_msg_rec.receive_message();

    assert (server_msg_rec.events.size() == events_first.size());
    for (std::size_t i = 0; i < events_first.size(); i++) {
        assert (events_first[i].type == server_msg_rec.events[i].type);
    }
    assert (server_msg_rec.game_id == server_msg.game_id);

    close(sock_client);
    close(sock_server);
}

void test_server_to_client_message_send_message_split_msg() {
    int sock_server, sock_client, game_id = 4684;
    int port_server = 54002, port_client = 54003;
    struct sockaddr_in server, client;

    Pixel px;
    NewGame game;

    game.maxx = 545846;
    game.maxy = 4648;
    game.players = vector<string>({"longnamea", "longnamea", "longnamea", "longnamea",
              "longnamea", "longnamea", "longnamea", "longnamea", "longnamea",
              "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea",
            "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
          "longnamea","longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
        ,"longnamea" ,"longnamea" ,"longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
      "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
    "longnamea", "longnamea", "longnamea", "longnameaaaaaaaaaa"});

    px.player_number = 1;
    px.x = 466;
    px.y = 466;

    Event e1, e2;

    e1.event_no = 1;
    e1.type = 0;
    e1.data = std::make_shared<NewGame>(game);

    e2.event_no = 2;
    e2.type = 1;
    e2.data = std::make_shared<Pixel>(px);

    vector<Event> events_all({
                  e1,
                  e2,
        });
    vector<Event> events_first = vector<Event>({
                e1
    });

    vector<Event> events_second = vector<Event>({
                e2
    });

    if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        assert (false);

    if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
        assert (false);

    memset((char *) &server, 0, sizeof(server));
    server.sin_family = PF_INET;
    server.sin_port = htons(port_server);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    memset((char *) &client, 0, sizeof(client));
    client.sin_family = PF_INET;
    client.sin_port = htons(port_client);

    if (inet_aton("127.0.0.1" , &client.sin_addr) == 0)
        assert (false);

    if (bind(sock_server, (struct sockaddr*)&server, sizeof(server)) == -1)
        assert (false);

    if (bind(sock_client, (struct sockaddr*)&client, sizeof(client)) == -1)
        assert (false);

    auto ptr = std::make_shared<struct sockaddr*>((struct sockaddr*)&client);

    ServerToClientMessage server_msg(sock_server, ptr);
    ServerToClientMessage server_msg_rec(sock_client, nullptr);

    server_msg.game_id = game_id;
    server_msg.events = events_all;

    server_msg.send_message(0);
    server_msg_rec.receive_message();

    assert (server_msg_rec.events.size() == events_first.size());
    for (std::size_t i = 0; i < events_first.size(); i++) {
        assert (events_first[i].type == server_msg_rec.events[i].type);
    }
    assert (server_msg_rec.game_id == server_msg.game_id);

    server_msg_rec.receive_message();

    assert (server_msg_rec.events.size() == events_second.size());
    for (std::size_t i = 0; i < events_second.size(); i++) {
        assert (events_second[i].type == server_msg_rec.events[i].type);
    }
    assert (server_msg_rec.game_id == server_msg.game_id);

    close(sock_client);
    close(sock_server);
}

void test_server_to_client_message_send_message_large_new_game() {
  int sock_server, sock_client, game_id = 4684;
  int port_server = 54002, port_client = 54003;
  struct sockaddr_in server, client;

  NewGame game;

  game.maxx = 545846;
  game.maxy = 4648;
  game.players = vector<string>({"longnamea", "longnamea", "longnamea", "longnamea",
            "longnamea", "longnamea", "longnamea", "longnamea", "longnamea",
            "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea",
          "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
        "longnamea","longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
      ,"longnamea" ,"longnamea" ,"longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
    "longnamea", "longnamea", "longnamea", "longnamea", "longnamea", "longnamea"
  "longnamea", "longnamea", "longnamea", "longnameaaaaaaaaaa",
"longnameaaaaaaaaaa","longnameaaaaaaaaaa",});

  Event e1;

  e1.event_no = 1;
  e1.type = 0;
  e1.data = std::make_shared<NewGame>(game);

  vector<Event> events_all({
                e1,
      });

  if ((sock_server = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
      assert (false);

  if ((sock_client = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
      assert (false);

  memset((char *) &server, 0, sizeof(server));
  server.sin_family = PF_INET;
  server.sin_port = htons(port_server);
  server.sin_addr.s_addr = htonl(INADDR_ANY);

  memset((char *) &client, 0, sizeof(client));
  client.sin_family = PF_INET;
  client.sin_port = htons(port_client);

  if (inet_aton("127.0.0.1" , &client.sin_addr) == 0)
      assert (false);

  if (bind(sock_server, (struct sockaddr*)&server, sizeof(server)) == -1)
      assert (false);

  if (bind(sock_client, (struct sockaddr*)&client, sizeof(client)) == -1)
      assert (false);

  auto ptr = std::make_shared<struct sockaddr*>((struct sockaddr*)&client);

  ServerToClientMessage server_msg(sock_server, ptr);
  ServerToClientMessage server_msg_rec(sock_client, nullptr);

  server_msg.game_id = game_id;
  server_msg.events = events_all;

  try {
    server_msg.send_message(0);
    close(sock_client);
    close(sock_server);
    assert (false);
  } catch (MessageTooLarge &e) {
    assert (true);
  } catch (...) {
    assert(false);
  }

  close(sock_client);
  close(sock_server);
}

void test_communicate() {
    test_server_from_client_message_load_valid();
    memset(buf, 0, sizeof(buf));
    test_server_from_client_message_load_invalid();
    memset(buf, 0, sizeof(buf));
    test_server_from_client_message_serialize();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_load_valid();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_load_invalid_crc();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_load_invalid_type();
    memset(buf, 0, sizeof(buf));
    test_ui_to_client_message_load();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_load_too_long();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_load_invalid_random_junk();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_send_message_simple();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_send_message_split_msg();
    memset(buf, 0, sizeof(buf));
    test_server_to_client_message_send_message_large_new_game();
    memset(buf, 0, sizeof(buf));
}
