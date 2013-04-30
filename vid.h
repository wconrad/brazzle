#ifndef VID_H
#define VID_H

#define VID_ROWS 25
#define VID_COLS 80
#define VID_DEFAULT_ATTR 0x07

void vid_clear();
void vid_clear_row(int row);
void vid_poke(int row, int col, char c, char attr);
void vid_scroll();

#endif
