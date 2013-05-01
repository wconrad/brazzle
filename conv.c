#include <stdbool.h>

#include "conv.h"
#include "string.h"

// Convert a signed decimal number to a string.
// buffer must be at least ITOA_BUFF_LEN long.
// Returns the pointer to the buffer.

char *
itoa(char * buffer, int n) {
  char * p = buffer;
  bool neg = (n < 0);
  unsigned u;
  if(neg)
    u = -n;
  else
    u = n;
  if(u == 0)
    *p++ = '0';
  else
    while(u != 0) {
      *p++ = u % 10 + '0';
      u /= 10;
    }
  if(neg)
    *p++ = '-';
  *p = '\0';
  return strrev(buffer);
}
