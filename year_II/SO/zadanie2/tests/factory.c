#include <unistd.h>
#include <stdio.h>
#include "factory.h"

#define VOLUME 10
#define QUANT (100 * 1000)

int produce(int64_t* p) {
  static int count = 0;

  usleep(QUANT);

  if (count < VOLUME) {
    int64_t created = 1;
    printf("PRODUCED: %ld\n", created);
    *p = created;
    count++;
    return 1;
  }

  return 0;
}

int consume(int64_t p) {
  static int count = 0;

  usleep(QUANT);

  if (count < VOLUME) {
    printf("CONSUMED: %ld\n", p);
    count++;
    return 1;
  }

  return 0;
}
