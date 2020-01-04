#ifndef SERIALIZE_TEXT_H
#define SERIALIZE_TEXT_H

class TextSerializable {
public:
    /* Serialize to textual format */
    virtual std::size_t serialize_text(char*) = 0;
};

#endif /* SERIALIZE_TEXT_H */
