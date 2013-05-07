#ifndef STRING_H
#define STRING_H

// Copy size bytes from src to dest.  Regions may not overlap.

void * memcpy(void * dest, const void * src, unsigned n);

// Copy size bytes from src to dest.  Regions may overlap.

void * memmove(void * dst, const void * src, unsigned size);

// Reverse a string in place.
// Returns s.

char * strrev(char * s);

// Return the length of a string.  Do not include the terminating NULL
// in the length.

unsigned strlen(const char * s);

// Set a region of memory.

unsigned memset(void * s, int c, unsigned length);

#endif
