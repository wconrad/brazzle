#include "vid.h"
#include "string.h"

// Low level video functions which interface directly with hardware.

// The video memory.

static char (* const video)[VID_ROWS][VID_COLS][2] = (void *) 0xb8000;

void vid_clear() {
  for (int row = 0; row < VID_ROWS; row++)
    vid_clear_row(row);
}

void vid_clear_row(int row) {
  for (int col = 0; col < VID_COLS; col++)
    vid_poke(row, col, ' ', VID_DEFAULT_ATTR);
}

void vid_poke(int row, int col, char c, char attr) {
  (*video)[row][col][0] = c;
  (*video)[row][col][1] = attr;
}

void vid_scroll() {
  memmove(&((*video)[0][0][0]),
          &((*video)[1][0][0]),
          sizeof(*video) - 2 * VID_COLS);
}
