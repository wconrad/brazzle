#ifndef CONV_H
#define CONV_H

#include <stddef.h>

// Convert an unsigned number to a decimal string.
// Will write no more than buflen characters to the buffer,
// including the terminating null.  Unless buflen is 0, the
// buffer is guaranteed to be null terminated on return./

// Returns the pointer to the buffer.

char *utodec(char * buffer, size_t buflen, unsigned n);

// Convert an unsigned number to a hex string.
// Returns a pointer to the buffer.

char *utohex(char * buffer, size_t buflen, unsigned n);

// Convert an unsigned number to a binary string.
// Returns the pointer to the buffer.

char *utobin(char * buffer, size_t buflen, unsigned n);

// Convert a signed number to a decimal string.
// Returns the pointer to the buffer.

char *itodec(char * buffer, size_t buflen, int n);

// Convert a string to an unsigned integer.
// On input, buffer points to a zero terminated string.
// On output, *endptr points to the first invalid character.
// buffer and *endptr may be equal, if desired.
// Returns the integer.

unsigned dectou(const char *buffer, const char **endptr);

#endif
