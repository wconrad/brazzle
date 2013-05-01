#include "vid.h"
#include "vty.h"

#include "sprintf.h"

// The cursor row (0...VID_ROWS)

static int row;

// The cursor column (0...VID_COLS)

static int col;

// The video attribute (foreground color, background color, blink,
// etc.)

static char attr;

// Move the cursor to the beginning of the line.

static void cr() {
  col = 0;
}

// Move the cursor to the beginning of the next line.  If it goes off the bottom of the screen,
// scroll the screen.

static void nl() {
  cr();
  if(row == VID_ROWS - 1) {
    vid_scroll();
    vid_clear_row(VID_ROWS - 1);
  }
  else
    row += 1;
}

// Move the cursor to the right.  If it goes off the right end of the
// line, move it to the beginning of the next line.  If it goes off
// bottom the screen, scroll the screen.

static void cursor_right() {
  col += 1;
  if (col >= VID_COLS)
    nl();
}

// Initialize.  Must be called before other function in this module.

void vty_init() {
  vid_clear();
  row = 0;
  col = 0;
  attr = VID_DEFAULT_ATTR;
}

// Write a character at the cursor location and advance the cursor.
// Special characters:
//   '\r': Move to beginning of this line
//   '\n': Move to beginning of next line

void vty_putc(char c) {
  switch(c) {
  case '\r':
    cr();
    break;
  case '\n':
    nl();
    break;
  default:
    vid_poke(row, col, c, attr);
    cursor_right();
  }
}

// Write a string.  Special characters are interpreted (see vty_putc).

void vty_puts(char * s) {
  char c;
  while((c = *s++) != '\0')
    vty_putc(c);  
}

// Write formatted.  Special characters are interpreted (see vty_putc).
// See vaprintf for formatting.

void vty_printf(const char * format, ...) {
  va_list varargs;
  va_start(varargs, format);
  char buffer[256];
  vsprintf(buffer, sizeof(buffer), format, varargs);
  vty_puts(buffer);
  va_end(varargs);
}
