#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "strdeque.h"
#include "strdequeconst.h"


int main() {
  int NUMBER = 10000000;

  for (int i=0; i<NUMBER; i++) {
    unsigned long d1 = strdeque_new();
    strdeque_delete(d1);
  }

  return 0;
}
