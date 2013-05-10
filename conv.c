#include <stdbool.h>

#include <stddef.h>

#include "buffwrite.h"
#include "conv.h"
#include "string.h"

// Convert an unsigned number to a string.
// radix may be between 2 and 16.
static void utoa(buffer_cursor_t * cursor,
                 unsigned n,
                 int radix) {
  char * start = cursor->p;
  if(n == 0)
    buffwrite(cursor, '0');
  else
    while(n != 0) {
      char c = "0123456789abcdef"[n % radix];
      buffwrite(cursor, c);
      n /= radix;
    }
  buffwrite(cursor, '\0');
  strrev(start);
}

char * utodec(char * buffer, size_t buflen, unsigned n) {
  buffer_cursor_t cursor = {.p = buffer, .remaining = buflen};
  utoa(&cursor, n, 10);
  return buffer;
}

char * utohex(char * buffer, size_t buflen, unsigned n) {
  buffer_cursor_t cursor = {.p = buffer, .remaining = buflen};
  utoa(&cursor, n, 16);
  return buffer;
}

char * utobin(char * buffer, size_t buflen, unsigned n) {
  buffer_cursor_t cursor = {.p = buffer, .remaining = buflen};
  utoa(&cursor, n, 2);
  return buffer;
}

char * itodec(char * buffer, size_t buflen, int n) {
  buffer_cursor_t cursor = {.p = buffer, .remaining = buflen};
  unsigned u;
  if(n < 0) {
    buffwrite(&cursor, '-');
    u = -n;
  }
  else
    u = n;
  utoa(&cursor, u, 10);
  return buffer;
}

unsigned dectou(const char *buffer, const char **endptr) {
  const char * p = buffer;
  unsigned n = 0;
  for(;;) {
    char c = *p;
    if(c < '0' || c > '9')
      break;
    p++;
    n = 10 * n + (c - '0');
  }
  *endptr = p;
  return n;
}
