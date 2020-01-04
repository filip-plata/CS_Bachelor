#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <pthread.h>
#include <assert.h>

#include "semaphore.h"
#include "factory.h"


#define INC 1000000


void *inc_val(void *val) {
  thread_data* data = (thread_data*) val;

  for (int i=0; i<INC; i++) {
    proberen(data->sem);
    data->val += 1;
    verhogen(data->sem);
  }

  return NULL;
}

void test_semaphore() {
  int32_t S = 1;
  pthread_t tid[thread_count];
  thread_data data = { .sem = &S, .val = 0};

  for (int i=1; i< thread_count; i++) {
    pthread_create(&tid[i], NULL, &inc_val, (void*)&data);
  }

  for (int i=1; i<thread_count; i++) {
    pthread_join(tid[i], NULL);
  }

  assert(data.val == (thread_count - 1) * INC);

  *(data.sem) = 2;

  for (int i=1; i< thread_count; i++) {
    pthread_create(&tid[i], NULL, &inc_val, (void*)&data);
  }

  for (int i=1; i<thread_count; i++) {
    pthread_join(tid[i], NULL);
  }

  assert(data.val != (thread_count - 1) * INC);

  printf("SEMAPHORE OK\n");
}

void test_init() {
  assert(init(0) == -2);

  for (size_t i = 1000; i<1500; i++) {
    assert(init(i) == 0);
  }

  assert(init(2147483648) == -1);
  assert(init(2147483649) == -1);
}

void* producer_thread(void* data) {
  producer();
  return NULL;
}

void* consumer_thread(void* data) {
    consumer();
    return NULL;
}

void test_producing() {
  size_t N = 5;

  size_t threads = 2;
  pthread_t tid[1 + threads];

  init(N);

  pthread_create(&tid[1], NULL, &producer_thread, NULL);
  pthread_create(&tid[2], NULL, &consumer_thread, NULL);

  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
}


int main() {
  test_semaphore();
  test_init();
  test_producing();
  return 0;
}
