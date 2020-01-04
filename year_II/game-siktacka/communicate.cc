#include <iostream>
#include <algorithm>

#include <cstring>

#include <unistd.h>

#include "utils.h"
#include "communicate.h"
#include "debug.h"


MessageTooLarge::MessageTooLarge(const std::string &msg)
        : msg_(msg) {};

const char * MessageTooLarge::what() const throw() {
    return msg_.c_str();
}

InvalidMessage::InvalidMessage(const std::string &msg)
    : message_(msg) {};

const char * InvalidMessage::what() const throw() {
    return message_.c_str();
}

Message::Message(int socket, std::shared_ptr<struct sockaddr*> send)
  : socket(socket) {
    if (send != nullptr)
        sender = send;
}

Message::Message(int socket)
  : Message(socket, nullptr) {}

Message::Message() {}

int Message::get_socket() const noexcept {
    return socket;
}

void Message::send_message_raw(std::size_t size) const {
    socklen_t slen(sizeof(struct sockaddr_in6));
    struct sockaddr* ss = *sender.get();
    struct sockaddr_in6 s6 = *((struct sockaddr_in6*) ss);
    char str[200];

    inet_ntop(PF_INET6, &(s6.sin6_addr), str, 200);
    DEBUG("Send to:") << str << std::endl;

    if (sendto(socket, buf, size, 0,
               *sender.get(), slen) == -1) {
        std::cerr << strerror(errno) << std::endl;
        if (errno == EWOULDBLOCK)
            throw WouldBlock();
        if (errno == ENETUNREACH || errno == ENETDOWN)
            throw NetworkUnreachable();
        /* Something weird has happened */
        throw std::system_error();
    }
}

std::size_t Message::receive_message_raw() {
    int msg_len(0);
    socklen_t slen(sizeof(info));

    if ((msg_len = recvfrom(socket, buf, BUF_SIZE, 0,
                            (struct sockaddr*)&info, &slen)) == -1) {
        std::cout << strerror(errno) << std::endl;
        throw std::system_error();
    }
    char str[10000];
    /* IP address is not set */
    inet_ntop(PF_INET6 ,&(info.sin6_addr), str, 100000);
    std::cout << "Received from : " << str << std::endl;

    return (std::size_t) msg_len;
}

void Message::clear_message() noexcept {
    std::memset(buf, 0, Message::BUF_SIZE);
}

void Message::send_message() {
    std::size_t size = serialize(buf);
    send_message_raw(size);
    clear_message();
}

void Message::receive_message() {
    std::size_t s = receive_message_raw();
    load(buf, s);

    if (invalid_message()) {
        clear_message();
        throw InvalidMessage(std::string(invalid_data_error_msg()));
    }
    clear_message();
}

std::string Message::invalid_data_error_msg() const {
    return "Invalid data send by: ";
}

ServerFromClientMessage::ServerFromClientMessage()
  : Message(), turn_direction(0) {}

ServerFromClientMessage::ServerFromClientMessage(int socket)
  : ServerFromClientMessage(socket, "", nullptr) {}

ServerFromClientMessage::ServerFromClientMessage(int socket,
        std::shared_ptr<struct sockaddr*> sender)
  : ServerFromClientMessage(socket, "", sender) {}

ServerFromClientMessage::ServerFromClientMessage(int socket,
        std::string pl, std::shared_ptr<struct sockaddr*> sender)
  : Message(socket, sender), session_id(time_now().count()), turn_direction(0),
    next_expected_event_no(1), player_name(pl) {
}


std::size_t ServerFromClientMessage::serialize(char *bufor) {
    std::size_t size(0);

    copy_to_raw_bytes(&session_id, bufor + size, &size);
    copy_to_raw_bytes(&turn_direction, bufor + size, &size);
    copy_to_raw_bytes(&next_expected_event_no, bufor + size, &size);

    const char *c = player_name.c_str();
    std::memcpy(bufor + size, c, player_name.length());
    size += player_name.length();

    return size;
}

 void ServerFromClientMessage::load(char *bufor, std::size_t dlen) {
    size_t size(0);

    copy_from_raw_bytes(&session_id, bufor + size, &size);
    copy_from_raw_bytes(&turn_direction, bufor + size, &size);
    copy_from_raw_bytes(&next_expected_event_no, bufor + size, &size);

    player_name = std::string(bufor + size);
    size += player_name.size();

    if (invalid_turn_direction(turn_direction) ||
        invalid_player_name(player_name) || size != dlen)
        throw InvalidLoad("Invalid data sended by server");
}

bool ServerFromClientMessage::invalid_message() noexcept {
    bool result = false;

    if (turn_direction != -1 && turn_direction != 0
        && turn_direction != 1)
        result = true;

    if (player_name.length() > 64)
        result = true;

    if (std::any_of(player_name.begin(), player_name.end(), [](char c) {
                int ascii = (int) c;
                return ascii < 33 || ascii > 126;
            }))
        result = true;

    return result;
}

ServerToClientMessage::ServerToClientMessage()
  : Message() {}

ServerToClientMessage::ServerToClientMessage(int socket,
      std::shared_ptr<struct sockaddr*> addr)
  : Message(socket, addr), events_server(nullptr) {}


bool ServerToClientMessage::invalid_message() noexcept {
    /* We assume server is correct */
    return false;
}

std::size_t ServerToClientMessage::serialize(char *buf) {
    std::size_t res(0);
    copy_to_raw_bytes(&game_id, buf, &res);
    return res;
}

void ServerToClientMessage::send_message(uint32_t last_event_no) {
    std::size_t size(0), new_s(0);
    size = serialize(buf);

    auto events_ptr = events_server != nullptr ? events_server : &events;

    if (last_event_no >= events_ptr->size())
        return;
    auto it = std::next(events_ptr->begin(), last_event_no);

    while (it != events_ptr->end()) {
        if (size == 0)
            size = serialize(buf);

        new_s = size + it->serialize(buf + size);
        if (new_s - size > ServerToClientMessage::MAX_MSG_SIZE - 4) {
            std::memset(buf + size, 0, Message::BUF_SIZE - size);
            std::cerr << "Very large NEW GAME ??" << std::endl;
            throw MessageTooLarge("Someone tries to hack server!");
        }
        if (new_s > ServerToClientMessage::MAX_MSG_SIZE) {
            send_message_raw(size);
            clear_message();
            size = serialize(buf);
        } else {
            size = new_s;
            it++;
        }
    }

    if (size > 0)
        send_message_raw(size);
    clear_message();
}


void ServerToClientMessage::load(char *buf, std::size_t dlen) {
    std::size_t size(0);
    long long len = dlen;

    events.clear();

    if (dlen > ServerToClientMessage::MAX_MSG_SIZE)
        throw InvalidLoad("Message is too long");

    copy_from_raw_bytes(&game_id, buf, &size);

    len -= size;

    while (len > 0) {
        Event e;
        try {
            e.load(buf + size, len);
            events.push_back(e);
        } catch (InvalidCrc &e) {
            break;
        } catch (InvalidType &e) {
        }
        size += (e.len + 8);
        len -= (e.len + 8);
    }

    if (len < 0)
        throw InvalidLoad("Strange parsing message failure: too short??");
}


void UiToClientMessage::load(char *buf, std::size_t dlen) {
    std::string message(buf);

    if (message == "LEFT_KEY_DOWN\n") {
        left_pressed = true;
    } else {
    if (message == "LEFT_KEY_UP\n") {
        left_pressed = false;
    } else {
    if (message == "RIGHT_KEY_UP\n") {
        rigth_pressed = false;
    } else {
    if (message == "RIGHT_KEY_DOWN\n") {
        rigth_pressed = true;
    } else {
        throw InvalidLoad("GUI sended wrong data");
      }
    }
    }
    }
    std::memset(this->buf, '\0', BUF_SIZE);

    if (left_pressed && !rigth_pressed) {
        turn_direction = -1;
    } else {
    if (!left_pressed && rigth_pressed) {
        turn_direction = 1;
    } else {
        turn_direction = 0;
    }
    }
}

std::size_t UiToClientMessage::serialize(char *buf) {
    std::size_t size(0);
    size += event->serialize_text(buf);
    return size;
}

UiToClientMessage::UiToClientMessage(int socket,
      std::shared_ptr<struct sockaddr*> sender)
    : Message(socket, sender), turn_direction(0),
      left_pressed(false), rigth_pressed(false) {
    std::memset(buf, '\0', BUF_SIZE);
}

UiToClientMessage::UiToClientMessage()
    : Message(), turn_direction(0),
      left_pressed(false), rigth_pressed(false) {
    std::memset(buf, '\0', BUF_SIZE);
}

void UiToClientMessage::send_message_raw(std::size_t size) {
    if (send(socket, buf, size, 0) == -1)
        if (errno == EWOULDBLOCK)
          throw WouldBlock();
}

std::size_t UiToClientMessage::receive_message_raw() {
    return recv(socket, buf, BUF_SIZE, 0);
}

void UiToClientMessage::receive_message() {
    std::size_t s = receive_message_raw();
    if (s == 0)
        throw GUIHangUp();

    try {
        load(buf, s);
    } catch (InvalidLoad &e) {
        std::string val(buf);
        std::memset(buf, '\0', BUF_SIZE);
        throw InvalidMessage("Invalid message send by GUI " + val);
    }
    clear_message();
}

bool UiToClientMessage::invalid_message() noexcept {
    return false;
}
