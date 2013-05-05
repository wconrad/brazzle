#ifndef CONV_H
#define CONV_H

// Conversion routines

// Buffer lengths
#define UTODEC_BUFF_LEN 11
#define UTOHEX_BUFF_LEN 9
#define ITODEC_BUFF_LEN 12
#define UTOBIN_BUFF_LEN 33

// Convert an unsigned number to a decimal string.
// buffer must be at least UTODEC_BUFF_LEN_HEX long.
// Returns the pointer to the buffer.

char *utodec(char * buffer, unsigned n);

// Convert an unsigned number to a hex string.
// buffer must be at least UTODEC_BUFF_LEN_HEX long.
// Returns the pointer to the buffer.

char *utohex(char * buffer, unsigned n);

// Convert an unsigned number to a binary string.
// buffer must be at least UTOBIN_BUFF_LEN_HEX long.
// Returns the pointer to the buffer.

char *utobin(char * buffer, unsigned n);

// Convert a signed number to a decimal string.
// buffer must be at least ITODEC_BUFF_LEN_DEC long.
// Returns the pointer to the buffer.

char *itodec(char * buffer, int n);

// Convert a string to an unsigned integer.
// On input, buffer points to a zero terminated string.
// On output, *endptr points to the first invalid character.
// buffer and *endptr may be equal, if desired.
// Returns the integer.

unsigned dectou(const char *buffer, const char **endptr);

#endif
