#ifndef BUFFWRITE_H
#define BUFFWRITE_H

// To avoid buffer overflow, which is a problem for both reliability and security,
// routines which write a character at a time to a buffer use
// buffwrite, which has safe behavior if the buffer gets full.

// An output buffer cursor, used by buffwrite.
typedef struct {
  char * p;
  int remaining;
} BufferCursor;

// Write a character to a buffer.  If the buffer only has room for one
// more character, write a '\0' instead.  If the buffer is full, do
// nothing.
void buffwrite(BufferCursor * cursor, char c);

#endif
