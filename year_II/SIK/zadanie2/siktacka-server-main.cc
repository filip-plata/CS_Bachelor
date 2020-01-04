#include <iostream>
#include <memory>

#include "siktacka-server.h"
#include "config.h"


int main(int argc, char *argv[]) {
    std::unique_ptr<config> conf;
    std::unique_ptr<Server> server;

    try {
        conf = std::make_unique<config>(argc, argv);
    } catch (std::exception &e) {
        std::cerr << "Usage ./siktacka-server [-W n]" <<
          " [-H n] [-p n] [-s n] [-t n] [-r n]" << std::endl;
        return 1;
    }

    try {
        server = std::make_unique<Server>(conf.get());
    } catch (std::system_error &e) {
        std::cerr << "Initialization of server failed" << std::endl;
        return 1;
    }

    server->run();

    return 0;
}
