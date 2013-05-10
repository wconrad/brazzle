#include <stdbool.h>

#include "buffwrite.h"
#include "conv.h"
#include "sprintf.h"
#include "string.h"

// Write a string to the sink
static void write_string_to_sink(vprintf_sink * sink,
                                 void * o,
                                 const char * s) {
  while (*s != '\0')
    sink(o, *s++);
}

// Write enough fill characters to make the field width come out to at
// least min_width.
static void vprintf_fill(vprintf_sink * sink,
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
      case 'b':
        converted = utobin(buffer, sizeof(buffer), va_arg(*varargs, int));
        break;
      case 'd':
        converted = itodec(buffer, sizeof(buffer), va_arg(*varargs, int));
        break;
      case 's':
        converted = va_arg(*varargs, char *);
        break;
      case 'u':
        converted = utodec(buffer, sizeof(buffer), va_arg(*varargs, unsigned));
        break;
      case 'x':
        converted = utohex(buffer, sizeof(buffer), va_arg(*varargs, int));
        break;
      case 'p':
        snprintf(buffer, sizeof(buffer), "0x%08x", va_arg(*varargs, unsigned));
        converted = buffer;
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
// * x - Unsigned integer, hexadecimal
// * p - Pointer
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

void vsprintf(char * buff,
              int bufflen,
              const char * format,
              va_list varargs) {
  buffer_cursor_t buffer_cursor = {.p = buff, .remaining = bufflen};
  vprintf((vprintf_sink *) buffwrite, &buffer_cursor, format, varargs);
}

void snprintf(char * buff, size_t bufflen, const char * format, ...) {
  va_list varargs;
  va_start(varargs, format);
  vsprintf(buff, bufflen, format, varargs);
  va_end(varargs);
}
