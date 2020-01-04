#include <iostream>
#include <vector>
#include <cstring>
#include <map>
#include <ctime>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <memory>
#include <chrono>
#include <stack>
#include <unistd.h>
#include <poll.h>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "circular.h"
#include "message.h"

static const int buffer_size = 4096;


struct config {
    uint16_t port;
    char *filename;
};

typedef struct config config;

int create_config(int argc, char *argv[], config *conf) {
    if (argc != 3)
        return -1;
    conf->port = S16(argv[1]);
    conf->filename = argv[2];
    return 0;
}


typedef std::pair<uint64_t, message_raw> Request;
typedef std::pair<uint64_t, sockaddr_in> ClientData;
typedef std::pair<message_raw, std::shared_ptr<std::stack<sockaddr_in>>> ReplyInfo;

bool operator< (const sockaddr_in a, const sockaddr_in b) {
    if (b.sin_addr.s_addr < a.sin_addr.s_addr)
        return true;
    if (b.sin_addr.s_addr > a.sin_addr.s_addr)
        return false;
    return b.sin_port < a.sin_port;
}

class Server {
public:
    Server(config conf)
        : buffer(buffer_size) {
        memset(&si_server, '\0', sizeof(si_server));

        if ((master_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
            throw std::runtime_error("Server: socket error");

        int enable = 1;
        if (setsockopt(master_socket, SOL_SOCKET, SO_REUSEADDR,
                       &enable, sizeof(int)) < 0)
            throw std::runtime_error("Server: setsockopt error");

        si_server.sin_family = AF_INET;
        si_server.sin_port = htons(conf.port);
        si_server.sin_addr.s_addr = htonl(INADDR_ANY);

        if (bind(master_socket, (struct sockaddr *) &si_server,
                 sizeof(si_server)) == -1)
            throw std::runtime_error("Server: bind error");

        std::stringstream buf;
        std::ifstream f(conf.filename);

        try {
            buf << f.rdbuf();
            file_content = buf.str();
        } catch (...) {
            throw std::runtime_error("Server: reading data from file");
        }
    }
    ~Server() { close(master_socket);}

    void run() {
        int activity(0);
        int timeout(-1);
        struct pollfd fd;
        fd.fd = master_socket;
        fd.events = POLLIN;

        while (true) {
            if (!buffer.empty())
                timeout = 0;
            else
                timeout = -1;

            activity = poll(&fd, 1, timeout);

            if (activity < 0)
                continue;

            if (activity)
                handle_data();
            else
                if (!buffer.empty())
                    send_reply();
        }
    }
private:
    int master_socket;
    struct sockaddr_in si_server;
    std::string file_content;
    circular_buffer<ReplyInfo> buffer;
    std::map<sockaddr_in, uint64_t> former_clients;

    std::stack<sockaddr_in> refresh_clients(uint64_t range = 120) {
        auto curr_time = std::time(nullptr);
        std::stack<sockaddr_in> result;
        std::map<sockaddr_in, uint64_t>::iterator it;

        for (it = former_clients.begin(); it != former_clients.end(); it++) {
            if (curr_time - it->second > range)
                former_clients.erase(it);
            else
                result.push(it->first);
        }

        return result;
    }

    void handle_data() {
        struct sockaddr_in si_client;
        message_raw msg;

        try {
            msg = receive_message(master_socket, &si_client);
        } catch (BadDatagram &e) {
            std::cerr << e.what() << std::endl;
            former_clients[si_client] = std::time(nullptr);
            return;
        }

        former_clients[si_client] = std::time(nullptr);
        buffer.push_back(std::make_pair(
                             msg,std::make_shared<std::stack<sockaddr_in> >(refresh_clients())));
    }

    void send_reply() {
        message_raw msg = append_data(buffer.front().first);

        if (!buffer.front().second->empty()) {
            send_message(master_socket,
                         buffer.front().second->top(), msg);
            buffer.front().second->pop();
        } else {
            buffer.pop_front();
        }
    }

    message_raw append_data(message_raw &msg) {
        if (file_content.length() > max_datagram_size) {
          std::cerr << "File too long" << std::endl;
          return msg;
        }

        msg.data = file_content;
        return msg;
    }
};


int main(int argc, char *argv[]) {
    config conf;
    std::unique_ptr<Server> server;

    if (create_config(argc, argv, &conf)) {
        std::cerr << "Usage: server port filename" << std::endl;
        exit(1);
    }

    try {
        server = std::unique_ptr<Server>(new Server(conf));
    } catch (std::exception &e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }

    server->run();

    return 0;
}
