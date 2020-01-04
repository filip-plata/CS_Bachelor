#ifndef UTILS_H
#define UTILS_H

#include <endian.h>    // __BYTE_ORDER
#include <algorithm>   // std::reverse
#include <stdexcept>
#include <chrono>
#include <cstring>

#include <netinet/in.h>
#include <sys/socket.h>

template <typename T>
constexpr T htonT (T value) noexcept
{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    char* ptr = reinterpret_cast<char*>(&value);
    std::reverse (ptr, ptr + sizeof(T));
    #endif
    return value;
}

template <typename T>
constexpr T ntohT (T value) noexcept
{
    #if __BYTE_ORDER == __LITTLE_ENDIAN
    char* ptr = reinterpret_cast<char*>(&value);
    std::reverse (ptr, ptr + sizeof(T));
    #endif
    return value;
}

template <typename T>
void copy_from_raw_bytes(T *data, char *buf, size_t *size) {
    T data_cp;
    std::memcpy(&data_cp, buf,
                sizeof(*data));
    *data = ntohT(data_cp);
    *size += sizeof(*data);
}

template <typename T>
void copy_to_raw_bytes(T *data, char *buf, size_t *size) {
    T data_cp = htonT(*data);
    std::memcpy(buf, &data_cp,
                sizeof(*data));
    *size += sizeof(*data);
}

template <typename T>
void check_port(T value) {
  if (value < std::numeric_limits<in_port_t>::min())
      throw std::invalid_argument("PORT too small");
  if (value > std::numeric_limits<in_port_t>::max())
      throw std::invalid_argument("PORT too large");
}

bool invalid_turn_direction(int8_t val);

bool invalid_player_name(std::string&);

std::chrono::microseconds time_now();

#endif /* UTILS_H */
