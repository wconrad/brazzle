#ifndef VID_H
#define VID_H

#define VID_ROWS 25
#define VID_COLS 80
#define VID_DEFAULT_ATTR 0x07

// Clear video.  Sets every character to a space with the default
// attribute.

void vid_clear();

// Clear a row.  Sets every character of the row to a space with the
// default attribute.

void vid_clear_row(int row);

// Set a character and attribute of video.

void vid_poke(int row, int col, char c, char attr);

// Scroll the screen up one line

void vid_scroll();

#endif
