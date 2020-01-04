#include <stdio.h>
#include <stdint.h>>
#include "stack_show.h"


extern void stack_show_test();
extern void stack_show();

void fifth_f() {
  stack_show();
}

void fourth_f() {
  char a[100];
  int b[20];

  fifth_f();
}

void third_f(unsigned int n) {
  if (n > 0) {
    third_f(--n);
  } else {
    fourth_f();
  }
}

void second_f() {
  third_f(0);
}

void first_f() {
  second_f();
}

int main() {
  printf("first_f %.16lx\n", (uint64_t) &first_f);
  printf("first_f %.16lx\n", (uint64_t) &second_f);
  printf("first_f %.16lx\n", (uint64_t) &third_f);
  printf("first_f %.16lx\n", (uint64_t) &fourth_f);
  
  first_f();
  return 0;
}
