#include <iostream>
#include <cstring>

#include <arpa/inet.h>
#include <unistd.h>
#include <netdb.h>
#include <memory>
#include <stdlib.h>
#include <system_error>

#include "message.h"

static uint64_t DEFAULT_PORT = 20160;


typedef struct config {
    uint64_t timestamp;
    char letter;
    char* host;
    uint16_t port;

    config()
      : port(DEFAULT_PORT) {};
} config;

typedef struct config config;

int create_config(int argc, char *argv[], config *conf) {
    if (argc != 4 && argc != 5)
        return -1;
    conf->timestamp = S64(argv[1]);

    if (!conf->timestamp)
        return 1;

    if (strlen(argv[2]) > 1)
        return 2;
    conf->letter = argv[2][0];

    conf->host = argv[3];

    if (argc == 5)
        conf->port = S16(argv[4]);

    if (!conf->port)
        return 3;
    return 0;
}

class MessengerClient {
private:
    int sock;
    struct sockaddr_in si_server;
    message_raw message;
public:
    MessengerClient(config conf)
        : message() {
        hostent *record = gethostbyname(conf.host);
        message_raw msg(conf.timestamp, conf.letter);

        if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            throw std::system_error(errno, std::system_category());

        if (record == NULL)
            throw std::system_error(errno, std::system_category());

        in_addr *address = (in_addr *)record->h_addr;

        si_server.sin_family = AF_INET;
        si_server.sin_port = htons(conf.port);
        si_server.sin_addr = *address;

        if (send_message(sock, si_server, msg))
            throw std::system_error(errno, std::system_category());
    };
    ~MessengerClient() { close(sock); }

    void receive() {
        struct sockaddr_in si_client;
        message = receive_message(sock, &si_client);
    }

    void print() {
        std::cout << message.head._timestamp << " "
                  << message.head._letter << " "
                  << message.data << std::endl;
    }
};


int main(int argc, char *argv[]) {
    config conf;
    std::unique_ptr<MessengerClient> msg_client;

    if (create_config(argc, argv, &conf)) {
        std::cerr << "Usage: client timestamp c host [port]" << std::endl;
        exit(1);
    }

    try {
        msg_client = std::unique_ptr<MessengerClient>(new MessengerClient(conf));
    } catch (std::exception) {
        exit(1);
    }

    while (true) {
        try {
            msg_client->receive();
            msg_client->print();
        } catch (BadDatagram &e) {
            std::cerr << e.what() << std::endl;
        }
    }

    return 0;
}
