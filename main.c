#include "vid.h"

int main() {
  vid_init();
  for (unsigned char c = ' '; c <= '~'; c++)
    vid_putc(c);
  for(;;) {}
}
