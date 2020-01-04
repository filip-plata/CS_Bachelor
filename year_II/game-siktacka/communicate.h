#ifndef COMMUNICATE_H
#define COMMUNICATE_H

#include <string>
#include <vector>
#include <memory>

#include <cstddef>
#include <cstdint>

#include <arpa/inet.h>
#include <sys/socket.h>

#include "serialize.h"
#include "event.h"


class MessageTooLarge : public std::exception {
public:
    explicit MessageTooLarge(const std::string &msg);
    virtual const char* what() const throw();
private:
    std::string msg_;
};


class InvalidMessage : public std::exception {
public:
    explicit InvalidMessage(const std::string &message);
    virtual const char* what() const throw();
private:
    std::string message_;
};

/* Exception to throw when sending data would block */
class WouldBlock : public std::exception {};
/* Exception GUI message manager should
   throw if GUI closes connection */
class GUIHangUp : public std::exception {};

/* Thrown be send_message_raw while network problems */
class NetworkUnreachable : public std::exception {};

/* Thrown when message of size < 512 cannot be sended */
class UnresolvableSend : public std::exception {};


class Message : public Serializable {
public:
    Message();
    Message(int socket);
    Message(int socket, std::shared_ptr<struct sockaddr*> sender);

    void receive_message();
    void send_message();

    int get_socket() const noexcept;

    int socket;
    struct std::shared_ptr<struct sockaddr*> sender;
    struct sockaddr_in6 info;
protected:
    static const std::size_t BUF_SIZE = 65536;
    char buf[BUF_SIZE];


    void clear_message() noexcept;
    virtual bool invalid_message() noexcept = 0;

    void send_message_raw(std::size_t) const;
    std::size_t receive_message_raw();
    std::string invalid_data_error_msg() const;
};

class ServerFromClientMessage : public Message {
public:
    uint64_t session_id;
    int8_t turn_direction;
    uint32_t next_expected_event_no;
    std::string player_name;

    ServerFromClientMessage();
    ServerFromClientMessage(int socket);
    ServerFromClientMessage(int socket, std::shared_ptr<struct sockaddr*>);
    ServerFromClientMessage(int socket, std::string,
            std::shared_ptr<struct sockaddr*>);

    std::size_t serialize(char *);
    void load(char *, std::size_t);
private:
    bool invalid_message() noexcept;
};

class ServerToClientMessage : public Message {
public:
    uint32_t game_id;
    /* This one can be quite suprising.
       Server will generate lots of events, and
       we would like to keep them in one place(in Server class).
       Client will have only a handful, and the simplest
       thing is to store them directly. */
    std::vector<Event> events;
    std::vector<Event> *events_server;

    ServerToClientMessage();
    ServerToClientMessage(int socket, std::shared_ptr<struct sockaddr*>);

    void send_message(uint32_t);

    std::size_t serialize(char *);
    void load(char *, std::size_t);
    static const std::size_t MAX_MSG_SIZE = 512;
private:
    bool invalid_message() noexcept;
};

class UiToClientMessage : public Message {
public:
    UiToClientMessage(int socket, std::shared_ptr<struct sockaddr*>);
    UiToClientMessage();

    int8_t turn_direction;
    std::shared_ptr<EventData> event;

    void receive_message();

    void load(char *, std::size_t);
    std::size_t serialize(char *);
private:
    bool left_pressed;
    bool rigth_pressed;
    bool invalid_message() noexcept;
    void send_message_raw(std::size_t);
    std::size_t receive_message_raw();
};

#endif /* COMMUNICATE_H */
