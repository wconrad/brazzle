#include "vid.h"
#include "vty.h"

static int row;
static int col;
static char attr;

// Move the cursor to the beginning of the line.

static void cr() {
  col = 0;
}

// Move the cursor to the beginning of the next line.  If it goes off the bottom of the screen,
// move it to the top of the screen.

static void nl() {
  cr();
  row = (row + 1) % VID_ROWS;
}

// Move the cursor to the right.  If it goes off the right end of the
// line, move it to the beginning of the next line.  If it goes off
// bottom the screen, move it to the top of the screen.

static void cursor_right() {
  col += 1;
  if (col >= VID_COLS)
    nl();
}

// Initialize.

void vty_init() {
  vid_clear();
  row = 0;
  col = 0;
  attr = VID_DEFAULT_ATTR;
}

// Write a character to the video, teletype style.

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

// Write a string to the video, teletype style.

void vty_puts(char * s) {
  char c;
  while((c = *s++) != '\0')
    vty_putc(c);  
}
