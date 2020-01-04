#include <iostream>
#include <memory>

#include "config.h"
#include "siktacka-client.h"


int main(int argc, char *argv[]) {
    std::unique_ptr<config_client> conf;
    std::unique_ptr<Client> client;

    try {
        conf = std::make_unique<config_client>(argc, argv);
    } catch (std::exception &e) {
        std::cerr << "Usage: ./siktacka-client player_name game_server_host[:port]" <<
          " [ui_server_host[:port]] " << std::endl;
        return 1;
    }

    try {
        client = std::make_unique<Client>(conf.get());
    } catch (std::system_error &e) {
        std::cerr << e.what() << std::endl;
        std::cerr << "Initialization of Client failed" << std::endl;
        return 1;
    }

    try {
        client->run();
    } catch (InvalidLoad &e) {
        std::cerr << "Invalid data send by server " << e.what() << std::endl;
        return 1;
    } catch (NetworkUnreachable &e) {
        std::cerr << "Game server or GUI server are unrechable: "
        << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Unknown error has occured" << std::endl;
    }
    return 0;
}
