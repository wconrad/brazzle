#include <stdbool.h>

#include "conv.h"
#include "string.h"

// Convert an unsigned decimal number to a string.
// buffer must be at least UTOA_BUFF_LEN long.
// Returns the pointer to the buffer.

char *
utoa(char * buffer, unsigned n) {
  char * p = buffer;
  if(n == 0)
    *p++ = '0';
  else
    while(n != 0) {
      *p++ = n % 10 + '0';
      n /= 10;
    }
  *p = '\0';
  return strrev(buffer);
}

// Convert a signed decimal number to a string.
// buffer must be at least ITOA_BUFF_LEN long.
// Returns the pointer to the buffer.

char *
itoa(char * buffer, int n) {
  char * p = buffer;
  bool neg = (n < 0);
  unsigned u;
  if(neg) {
    *p++ = '-';
    u = -n;
  }
  else
    u = n;
  utoa(p, u);
  return buffer;
}
