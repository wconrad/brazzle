#include <stdbool.h>

#include "conv.h"
#include "string.h"

// Convert an unsigned number to a string.
// buffer must be at least UTODEC_BUFF_LEN_HEX long.
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

// Convert an unsigned number to a decimal string.
// buffer must be at least UTODEC_BUFF_LEN_HEX long.
// Returns the pointer to the buffer.

char *
utodec(char * buffer, unsigned n) {
  return utoa(buffer, n, 10);
}

// Convert an unsigned number to a hex string.
// buffer must be at least UTODEC_BUFF_LEN_HEX long.
// Returns the pointer to the buffer.

char *
utohex(char * buffer, unsigned n) {
  return utoa(buffer, n, 16);
}

// Convert a signed number to a decimal string.
// buffer must be at least ITODEC_BUFF_LEN_DEC long.
// Returns the pointer to the buffer.

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

// Convert a string to an unsigned integer.
// On input, buffer points to a zero terminated string.
// On output, *endptr points to the first invalid character.
// buffer and *endptr may be equal, if desired.
// Returns the integer.

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
