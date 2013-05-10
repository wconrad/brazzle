#include "buffwrite.h"

// Write a character to a buffer.  If the buffer only has room for one
// more character, write a '\0' instead.  If the buffer is full, do
// nothing.

void buffwrite(buffer_cursor_t * cursor, char c) {
  if(cursor->remaining <= 0)
    return;
  if(cursor->remaining == 1)
    c = '\0';
  *(cursor->p)++ = c;
  cursor->remaining--;
}
