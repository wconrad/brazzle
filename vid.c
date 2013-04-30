#include "vid.h"

// Low level video functions which interface directly with hardware.

// The video memory.

static char (* const video)[VID_ROWS][VID_COLS][2] = (void *) 0xb8000;

// Set a character and attribute of video.

void vid_poke(int row, int col, char c, char attr) {
  (*video)[row][col][0] = c;
  (*video)[row][col][1] = attr;
}

// Clear video.  Sets every character to a space with the default
// attribute.

void vid_clear() {
  for (int row = 0; row < VID_ROWS; row++)
    for (int col = 0; col < VID_COLS; col++)
      vid_poke(row, col, ' ', VID_DEFAULT_ATTR);
}
