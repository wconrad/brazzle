#include "string.h"

// Copy size bytes from src to dst.  The regions may overlap, so long
// as src >= dst.

static void *
memcpy_forward(void * dst, const void * src, unsigned size) {
  for(unsigned i = 0; i < size; i++)
    ((char *)dst)[i] = ((const char *)src)[i];
  return dst;
}

// Copy size bytes from src to dst.  The regions may overlap, so long
// as src <= dst.

static void *
memcpy_backward(void * dst, const void * src, unsigned size) {
  for(unsigned i = 0; i < size; i++)
    ((char *)dst)[(size - 1) - i] = ((const char *)src)[(size - 1) - i];
  return dst;
}

// Copy size bytes from src to dest.  Regions may not overlap.

void *
memcpy(void * dst, const void * src, unsigned size) {
  return memcpy_forward(dst, src, size);
}

// Copy size bytes from src to dest.  Regions may overlap.

void *
memmove(void * dst, const void * src, unsigned size) {
  if(src >= dst)
    return memcpy_forward(dst, src, size);
  else
    return memcpy_backward(dst, src, size);
}

// Reverse a string in place.
// Returns s.

char *
strrev(char * s) {
  char * a = s;
  char * b = s;
  while(*b != '\0')
    b++;
  b--;
  while (a < b) {
    char temp = *a;
    *a++ = *b;
    *b-- = temp;
  }
  return s;
}
