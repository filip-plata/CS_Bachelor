#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "strdeque.h"
#include "strdequeconst.h"


int main() {
  unsigned long d1;

  d1 = strdeque_new();
  char buf[4] = "foo";
  strdeque_insert_at(d1, 0, buf);
  buf[0] = 'b';

  assert(strdeque_get_at(d1, 0) != NULL);
  assert(strncmp(strdeque_get_at(d1, 0), "foo", 4) == 0);
  assert(strncmp(strdeque_get_at(d1, 0), "boo", 4) != 0);

  return 0;
}
