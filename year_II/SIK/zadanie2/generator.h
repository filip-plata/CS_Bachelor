#ifndef GENERATOR_H
#define GENERATOR_H

#include <cstdint>

typedef uint64_t gen_t;

gen_t next();
void set_seed(gen_t);

#endif /* GENERATOR_H */
