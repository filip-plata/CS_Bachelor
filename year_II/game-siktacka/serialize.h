#ifndef SERIALIZE_H
#define SERIALIZE_H

#include <exception>

class InvalidLoad : public std::exception {
public:
    InvalidLoad(std::string const &message) : msg_(message) {};
    virtual char const *what() const noexcept {  return msg_.c_str(); };
private:
    std::string msg_;
};


class Serializable {
public:
    /* Convert object to raw bytes into buffer.
       Returns number of serialized bytes */
    virtual std::size_t serialize(char *buf) = 0;
    /* Loads object from raw bytes. dlen specifies
       how many bytes tu use are in buffer. */
    virtual void load(char *buf, std::size_t dlen) = 0;
};

#endif /* SERIALIZE_H */
