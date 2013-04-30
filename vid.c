#include "vid.h"

static int row;
static int col;
static unsigned char attr;

static unsigned char (* const video)[VID_ROWS][VID_COLS][2] = (void *) 0xb8000;

// Set a character and attribute of video.

static void vid_poke(int row, int col, unsigned char c, unsigned char attr) {
  (*video)[row][col][0] = c;
  (*video)[row][col][1] = attr;
}

// Initialize video.

void vid_init() {
  row = 0;
  col = 0;
  attr = 0x07;
}

// Write a character to the video, teletype style.

void vid_putc(unsigned char c) {
  vid_poke(row, col, c, attr);
  col = (col + 1) % VID_COLS;
  if(col == 0)
    row = (row + 1) % VID_ROWS;
}
