#include <string>
#include <cstring>
#include <stdexcept>
#include <iostream>
#include <limits>
#include <ctime>

#include <unistd.h>
#include <netdb.h>

#include "config.h"
#include "IpAddress.h"
#include "utils.h"
#include "debug.h"


config::config()
    : ROUNDS_PER_SEC(50), TURNING_SPEED(6),
      WIDTH(800), HEIGHT(600), PORT(12345), SEED(time(nullptr)) {};

config::config(int argc, char *argv[])
    : config() {
    char c;
    long long value;

    while ((c = getopt(argc, argv, "W:H:p:s:t:r:")) != -1) {
        value = std::stoll(std::string(optarg));
        if (c != 'r' && value < 0)
	  throw std::invalid_argument("Negative values for arg: " + std::string(1, c));
        switch (c) {
        case 'W':
            if (value < std::numeric_limits<uint32_t>::min())
                throw std::invalid_argument("WIDTH too small");
            if (value > std::numeric_limits<uint32_t>::max())
                throw std::invalid_argument("WIDTH too large");
            WIDTH = value;
            break;
        case 'H':
            if (value < std::numeric_limits<uint32_t>::min())
                throw std::invalid_argument("HEIGHT too small");
            if (value > std::numeric_limits<uint32_t>::max())
                throw std::invalid_argument("HEIGHT too large");
            HEIGHT = value;
            break;
        case 'p':
            check_port(value);
            PORT = value;
            break;
        case 's':
            ROUNDS_PER_SEC = value;
            break;
        case 't':
            TURNING_SPEED = value;
            break;
        case 'r':
            SEED = (gen_t) value;
            break;
        default:
            throw std::invalid_argument("Invalid cl argument");
            break;
        }
    }
}


static void set_port(struct sockaddr *addr, in_port_t port) {
    if (addr->sa_family == AF_INET) {
        ((struct sockaddr_in*) addr)->sin_port = htons(port);
    }
    if (addr->sa_family == AF_INET6) {
        ((struct sockaddr_in6*) addr)->sin6_port = htons(port);
    }
}

static void parse_hostname_string(
      std::string hostname,
      struct sockaddr **addr,
      in_port_t default_port) {
    struct addrinfo hints, *info, *p;
    int r;
    DEBUG("Hostname: ") << hostname;
    IpAddress ip_address = IpAddress::Parse(hostname);
    DEBUG("IP type: ") << ip_address.getType();
    if (ip_address.getType() == IpAddress::Unknown)
        throw std::invalid_argument("Wrong format of ip address");

    memset(&hints, '\0', sizeof(hints));
    hints.ai_family = AF_UNSPEC;

    DEBUG("IP address: ") << ip_address.getHostAddress() << std::endl;

    r = getaddrinfo(ip_address.getHostAddress().c_str(), NULL, &hints, &info);

    if (r < 0)
      throw std::invalid_argument("getaddrinfo could not resolve host");

    for(p = info; p != NULL; p = p->ai_next) {
        /* We use last address */
        *addr = p->ai_addr;
    }

    if (ip_address.getPortNumber() != 0) {
        check_port(ip_address.getPortNumber());
        set_port(*addr, ip_address.getPortNumber());
    }
    else {
        set_port(*addr, default_port);
    }

    //freeaddrinfo(info);
}


config_client::config_client(int argc, char *argv[]) {
    if (argc < 3)
      throw std::invalid_argument("Not enough arguments");
    if (argc > 4)
      throw std::invalid_argument("Too many arguments");

    player_name = std::string(argv[1]);
    std::string ui_server_string("localhost");

    parse_hostname_string(std::string(argv[2]), &game_server, 12345);

    if (argc == 4)
      ui_server_string = std::string(argv[3]);

    parse_hostname_string(ui_server_string, &ui_server, 12346);

    if (invalid_player_name(player_name))
        throw std::invalid_argument("Invalid player name");
}
