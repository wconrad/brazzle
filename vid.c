#include "vid.h"

static int row;
static int col;
static char attr;

static char (* const video)[VID_ROWS][VID_COLS][2] = (void *) 0xb8000;

// Set a character and attribute of video.

static void vid_poke(int row, int col, char c, char attr) {
  (*video)[row][col][0] = c;
  (*video)[row][col][1] = attr;
}

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

// Initialize video.

void vid_init() {
  row = 0;
  col = 0;
  attr = 0x07;
}

// Clear video.

void vid_clear() {
  for (int row = 0; row < VID_ROWS; row++)
    for (int col = 0; col < VID_COLS; col++)
      vid_poke(row, col, ' ', attr);
}

// Write a character to the video, teletype style.

void vid_putc(char c) {
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

void vid_puts(char * s) {
  char c;
  while((c = *s++) != '\0')
    vid_putc(c);  
}
