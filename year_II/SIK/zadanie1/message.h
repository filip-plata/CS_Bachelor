#ifndef MESSAGE_H
#define MESSAGE_H

#include <inttypes.h>
#include <ctime>
#include <cstring>
#include <iostream>
#include <sstream>

#define htonll(x) ((1==htonl(1)) ? (x) : \
((uint64_t)htonl((x) & 0xFFFFFFFF) << 32) | (uint64_t)htonl((x) >> 32))
#define ntohll(x) ((1==ntohl(1)) ? (x) : \
((uint64_t)ntohl((x) & 0xFFFFFFFF) << 32) | (uint64_t)ntohl((x) >> 32))

static const size_t max_datagram_size = 65507;
static const size_t buffor_size = 65560;
static const uint64_t world_end_year = 71697398400;

class BadDatagram: public std::exception {
private:
    std::string message_;
public:
    BadDatagram(const std::string &message)
        : message_(message) {};
    virtual const char* what() const throw() {
        return message_.c_str();
    }
};

uint64_t S64(char *s) {
    uint64_t i;
    std::istringstream ss(s);
    ss >> i;
    return i;
}

uint16_t S16(char *s) {
    uint16_t i;
    std::istringstream ss(s);
    ss >> i;
    return i;
}

typedef struct message_header {
    uint64_t _timestamp;
    char _letter;

    message_header() {
      _timestamp = 0;
      _letter = '\0';
    }

    message_header(uint64_t timestamp, char letter)
      : _timestamp(timestamp), _letter(letter) {};
} message_header;

typedef struct message_raw {
    message_header head;
    std::string data;

    message_raw()
      : head(), data("") {};

    message_raw(uint64_t timestamp, char letter)
      : head(timestamp, letter) {};
} message_raw;

int isinvalid_message(message_raw msg) {
    /* If received letter is null, we simply return error
       since it is too much trouble. */
    time_t a = msg.head._timestamp;

    int year = gmtime(&a)->tm_year + 1900;

    return year < 1717 || year > 4242;
}

void int64ToChar(char mesg[], uint64_t num) {
    for(int i = 0; i < 8; i++) mesg[i] = num >> (7-i)*8;
}

int send_message(int sock_fd, struct sockaddr_in addr_in, message_raw msg) {
    static char buf[buffor_size];
    static char bytes[8];
    int64ToChar(bytes, htonll(msg.head._timestamp));
    for (int i=0; i<8; i++) {
        buf[i] = bytes[i];
    }
    buf[8] = msg.head._letter;
    char *ptr = &buf[9];

    sprintf(ptr, "%s", msg.data.c_str());

    if (sendto(sock_fd, buf, 9 + msg.data.length(), 0,
               (struct sockaddr *) &addr_in, sizeof(addr_in)) == -1)
        return 1;

    memset(buf, '\0', buffor_size);
    return 0;
}

uint64_t charTo64bitNum(char a[]) {
    uint64_t n = 0;
    n = (((uint64_t)a[0] << 56) & 0xFF00000000000000U)
        | (((uint64_t)a[1] << 48) & 0x00FF000000000000U)
        | (((uint64_t)a[2] << 40) & 0x0000FF0000000000U)
        | (((uint64_t)a[3] << 32) & 0x000000FF00000000U)
        | ((a[4] << 24) & 0x00000000FF000000U)
        | ((a[5] << 16) & 0x0000000000FF0000U)
        | ((a[6] <<  8) & 0x000000000000FF00U)
        | (a[7]        & 0x00000000000000FFU);
    return n;
}

message_raw receive_message(int sock_fd, struct sockaddr_in *si_sender) {
    static char buf[buffor_size];
    static char a[8];
    message_raw msg;
    socklen_t slen = sizeof(si_sender);
    recvfrom(sock_fd, buf, buffor_size, 0,
            (struct sockaddr *) si_sender, &slen);

    for (int i=0; i<8; i++) {
        a[i] = buf[i];
    }
    msg.head._timestamp = charTo64bitNum(a);
    msg.head._letter = buf[8];

    char *ptr = &buf[9];
    std::string rest(ptr);
    msg.data = rest;

    msg.head._timestamp = ntohll(msg.head._timestamp);

    if (isinvalid_message(msg))
        throw BadDatagram(std::string("Invalid datagram send by: ") +
                          std::string(inet_ntoa(si_sender->sin_addr)));

    memset(buf, '\0', buffor_size);
    return msg;
}

#endif /* MESSAGE_H */
