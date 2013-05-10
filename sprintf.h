#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdarg.h>
#include <stddef.h>

// vprintf expects a function of this type.  The function should
// accept a character of formatted output and do something with it
// (e.g. print, store, etc.).
//
// * o is a pointer given to vprintf and passed in turn to to
//   vprintf_sink.  It can point to, e.g., an
//   output buffer.
// * c is the character.
typedef void vprintf_sink(void * o, char c);

// Format a string, using rules somewhat like standard C's sprintf
// function.
//
// A format specifier indicates that a variable from the va_list
//
// Format contains literal text intersperced with format specifiers.
// A format specifier indicates that a variable from va_list should be
// formatted and inserted at that point.
//
// Here's the regular expression for a format specifier:
//   %-?0?\d*[dsu]
// If the '-' is present, the field is left justified, otherwise right
// If the '0' is present, the field is zero-filled, otherwise space filled
// If the digits are present, the field is filled up to that width
// The last digit stands for the type:
// * d - Signed integer
// * s - String
// * u - Unsigned integer
// * x - Unsigned integer, hexadecimal
// * p - Pointer
// To print a % sign, use %%

void snprintf(char * buff, size_t bufflen, const char * format, ...);

// Format taking a buffer and va_list.  See sprintf for format specifiers.
void vsprintf(char * buff,
              int bufflen,
              const char * format,
              va_list varargs);

#endif
