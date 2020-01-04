#include <ctime>

#include "generator.h"

static gen_t value = std::time(nullptr);
static const gen_t MAGIC1 = 279470273;
static const gen_t MAGIC2 = 4294967291;


void set_seed(gen_t val) {
    value = val;
}

gen_t next() {
    uint64_t ret = value;
    value = (value * MAGIC1) % MAGIC2;
    return ret;
}
