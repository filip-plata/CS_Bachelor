#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "strdeque.h"
#include "strdequeconst.h"

void insert_to_deque(unsigned long id, size_t pos) {
  const char* buf = "foo";
  strdeque_insert_at(id, pos, buf);
}


int main() {
  // inserting out of scope to deque
  unsigned long d1;

  d1 = strdeque_new();

  insert_to_deque(d1, 0);
  assert(strncmp(strdeque_get_at(d1, 0), "foo", 4) == 0);

  return 0;
}
