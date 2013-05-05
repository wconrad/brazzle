#include <stdbool.h>

#include "conv.h"
#include "string.h"

// Convert an unsigned number to a string.
// radix may be between 2 and 16.
// Returns the pointer to the buffer.

static char *
utoa(char * buffer, unsigned n, int radix) {
  char * p = buffer;
  if(n == 0)
    *p++ = '0';
  else
    while(n != 0) {
      *p++ = "0123456789abcdef"[n % radix];
      n /= radix;
    }
  *p = '\0';
  return strrev(buffer);
}

char *
utodec(char * buffer, unsigned n) {
  return utoa(buffer, n, 10);
}

char *
utohex(char * buffer, unsigned n) {
  return utoa(buffer, n, 16);
}
char *
utobin(char * buffer, unsigned n) {
  return utoa(buffer, n, 2);
}

char *
itodec(char * buffer, int n) {
  char * p = buffer;
  bool neg = (n < 0);
  unsigned u;
  if(neg) {
    *p++ = '-';
    u = -n;
  }
  else
    u = n;
  utodec(p, u);
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
