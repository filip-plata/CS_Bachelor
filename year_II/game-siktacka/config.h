#ifndef CONFIG_H
#define CONFIG_H

#include <string>

#include <cstdint>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "generator.h"

struct config {
    long long ROUNDS_PER_SEC;
    long long TURNING_SPEED;
    uint32_t WIDTH;
    uint32_t HEIGHT;
    in_port_t PORT;
    gen_t SEED;

    config();
    config(int argc, char *argv[]);
};

struct config_client {
    std::string player_name;
    struct sockaddr *game_server;
    struct sockaddr *ui_server;

    config_client(int argc, char *argv[]);
};

#endif /* CONFIG_H */
