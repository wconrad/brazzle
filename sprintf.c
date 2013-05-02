#include <stdbool.h>

#include "conv.h"
#include "sprintf.h"
#include "string.h"

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

// Format a signed integer.

static char *
vprintf_signed(char * buffer, va_list *varargs) {
  int n = va_arg(*varargs, int);
  itodec(buffer, n);
  return buffer;
}

// Format an unsigned integer;

static char *
vprintf_unsigned(char * buffer, va_list *varargs) {
  int n = va_arg(*varargs, int);
  utodec(buffer, n);
  return buffer;
}

// Format a string.

static char *
vprintf_string(char * buffer, va_list * varargs) {
  (void) buffer;
  return va_arg(*varargs, char *);
}

// Write a string to the sink

static void
write_string_to_sink(vprintf_sink * sink, void * o, const char * s) {
  while (*s != '\0')
    sink(o, *s++);
}

static void
vprintf_fill(vprintf_sink * sink,
             void * o,
             char fill_char,
             unsigned min_width,
             const char * converted) {
  unsigned converted_length = strlen(converted);
  while(converted_length < min_width) {
    sink(o, fill_char);
    converted_length++;
  }
  
}

// Format a directive.  Returns the new formatp.

static const char * 
vprintf_directive(vprintf_sink * sink,
                  void * o,
                  const char * formatp,
                  va_list *varargs) {
  char c = *formatp;
  switch(c) {
  case '\0':
    sink(o, '%');
    break;
  case '%':
    formatp++;
    sink(o, '%');
    break;
  default:
    {
      // buffer must be large enough for _any_ of the functions we
      // pass it to.
      char buffer[64];
      char * converted;
      char fill_char = ' ';
      bool left_justify = false;
      if(*formatp == '-') {
        formatp++;
        left_justify = true;
      }
      if(*formatp == '0')
        fill_char = *formatp++;
      unsigned min_width = dectou(formatp, &formatp);
      c = *formatp++;
      switch(c) {
      case 'd':
        converted = vprintf_signed(buffer, varargs);
        break;
      case 's':
        converted = vprintf_string(buffer, varargs);
        break;
      case 'u':
        converted = vprintf_unsigned(buffer, varargs);
        break;
      default:
        buffer[0] = '%';
        buffer[1] = c;
        buffer[2] = '\0';
        converted = buffer;
      }
      if(!left_justify)
        vprintf_fill(sink, o, fill_char, min_width, converted);
      write_string_to_sink(sink, o, converted);
      if(left_justify)
        vprintf_fill(sink, o, fill_char, min_width, converted);
    }
  }
  return formatp;
}

// Format taking a sink and va_list.
// A format specifier indicates that a variable from the va_list
// should be formatted and inserted at that point.
// Here's the regular expression for a format specifier:
//   %-?0?\d*[dsu]
// If the '-' is present, the field is left justified, otherwise right
// If the '0' is present, the field is zero-filled, otherwise space
// If the digits are present, the field is filled up to that width
// The last digit stands for the type:
// * d - Signed integer
// * s - String
// * u - Unsigned integer
// To print a % sign, use %%

void vprintf(vprintf_sink * sink,
             void * o,
             const char * format,
             va_list varargs) {
  const char * formatp = format;
  for(;;) {
    char c = *formatp++;
    if(c == '%')
      formatp = vprintf_directive(sink, o, formatp, &varargs);
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
