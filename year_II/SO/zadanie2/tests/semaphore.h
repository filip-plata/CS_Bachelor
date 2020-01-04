#ifndef SEMAPHORE_H
#define SEMAPHORE_H

void proberen(int32_t*);
void verhogen(int32_t*);

static int thread_count = 5;

struct thread_data {
  int32_t* sem;
  int64_t val;
};

struct factory_data {
  int32_t* producer;
  int32_t* consumer;
  int32_t* mutex;
};

typedef struct thread_data thread_data;
typedef struct factory_data factory_data;

#endif /* SEMAPHORE_H */
