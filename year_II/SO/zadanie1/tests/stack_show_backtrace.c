#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>

#define buffer_len ((size_t) 100)


void stack_show_test() {
  static void *buffer[buffer_len];
  char **strings;
  size_t trace_len;

  trace_len = backtrace(buffer, buffer_len);
  strings = backtrace_symbols(buffer, trace_len);

  for (size_t i = 1; i < trace_len; i++) {
    printf("%p %s\n", buffer[i], strings[i]);
  }
}
