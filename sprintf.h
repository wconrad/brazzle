#ifndef SPRINTF_H
#define SPRINTF_H

#include <stdarg.h>

// vprintf expects a function of this type.  The function should
// accept a character of formatted output and do something with it
// (e.g. print, store, etc.).
//
// * o is the pointer given to vprintf.  It can point to, e.g., an
//   output buffer.
// * c is the character.

typedef void vprintf_sink(void * o, char c);

void sprintf(char * buff, int bufflen, const char * format, ...);
void vsprintf(char * buff,
              int bufflen,
              const char * format,
              va_list varargs);

#endif
