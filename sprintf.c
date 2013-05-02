#include <stdbool.h>

#include "conv.h"
#include "sprintf.h"

// An output buffer cursor, used by sprintf and buffwrite to keep
// track of the write pointer and the space remaining.

typedef struct {
  char * p;
  int remaining;
} buffer_cursor_t;

// Write a character to a buffer.  If the buffer only has room for one
// more character, write a '\0' instead.  If the buffer is full, do
// nothing.

static void buffwrite(buffer_cursor_t * cursor, char c) {
  if(cursor->remaining <= 0)
    return;
  if(cursor->remaining == 1)
    c = '\0';
  *(cursor->p)++ = c;
  cursor->remaining--;
}

// Format a signed decimal number.

static void
vprintf_decimal(vprintf_sink * sink,
                void * o,
                va_list varargs) {
  int n = va_arg(varargs, int);
  char buffer[ITODEC_BUFF_LEN_DEC];
  char * p = itodec(buffer, n);
  while(*p != '\0')
    sink(o, *p++);
}

// Format a string.

static void
vprintf_string(vprintf_sink * sink,
               void * o,
               va_list varargs) {
  char * s = va_arg(varargs, char *);
  while(*s != '\0')
    sink(o, *s++);
}

// Format a directive.  Returns the new formatp.

static const char * 
vprintf_directive(vprintf_sink * sink,
                  void * o,
                  const char * formatp,
                  va_list varargs) {
  char c = *formatp++;
  switch(c) {
  case '\0':
    formatp--;
    sink(o, '%');
    break;
  case '%':
    sink(o, c);
    break;
  case 'd':
    vprintf_decimal(sink, o, varargs);
    break;
  case 's':
    vprintf_string(sink, o, varargs);
    break;
  default:
    sink(o, '%');
    sink(o, c);
  }
  return formatp;
}

// Format taking a sink and va_list.
// Format specifiers:
//   %% - Print a percent sign
//   %d - Format decimal
//   %s - Format string

void vprintf(vprintf_sink * sink,
             void * o,
             const char * format,
             va_list varargs) {
  const char * formatp = format;
  for(;;) {
    char c = *formatp++;
    if(c == '%')
      formatp = vprintf_directive(sink, o, formatp, varargs);
    else {
      sink(o, c);
      if(c == '\0')
        break;
    }
  }
}

// Format taking a buffer and va_list.  See vsprintf for format
// specifiers.

void vsprintf(char * buff,
              int bufflen,
              const char * format,
              va_list varargs) {
  buffer_cursor_t buffer_cursor = {.p = buff, .remaining = bufflen};
  vprintf((vprintf_sink *) buffwrite, &buffer_cursor, format, varargs);
}

// Formating taking a buffer and variable arguments.  See vsprintf for
// format specifiers.

void sprintf(char * buff, int bufflen, const char * format, ...) {
  va_list varargs;
  va_start(varargs, format);
  vsprintf(buff, bufflen, format, varargs);
  va_end(varargs);
}
