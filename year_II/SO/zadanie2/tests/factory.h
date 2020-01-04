#ifndef FACTORY_H
#define FACTORY_H

#include <stdint.h>
#include <stddef.h>

int init(size_t N);
void producer();
void consumer();

int produce(int64_t*);
int consume(int64_t);

#endif /* FACTORY_H */
