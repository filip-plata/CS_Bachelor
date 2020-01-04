#include <cassert>
#include <cstdint>
#include <iostream>
#include "../utils.h"
#include "test_utils.h"

static const std::size_t BSIZE = 100;
static char buf[BSIZE];

void test_utils() {
    std::size_t size(0);
    uint32_t value = 100;
    copy_to_raw_bytes(&value, buf, &size);

    assert (value == 100);

    assert (buf[0] == 0x00);
    assert (buf[1] == 0x00);
    assert (buf[2] == 0x00);
    assert (buf[3] == 0x64);

    uint32_t copy = 200;

    copy_from_raw_bytes(&copy, buf, &size);

    assert(copy == 100);

    size = 0;
    uint64_t val = 7568126430489LL;
    copy_to_raw_bytes(&val, buf, &size);

    assert (size == 8);
    size = 0;
    assert (buf[size++] == (char) 0x00);
    assert (buf[size++] == (char) 0x00);
    assert (buf[size++] == (char) 0x06);
    assert (buf[size++] == (char) 0xE2);
    assert (buf[size++] == (char) 0x17);
    assert (buf[size++] == (char) 0x7C);
    assert (buf[size++] == (char) 0xCD);
    assert (buf[size++] == (char) 0x19);
}
