#include "vid.h"

int main() {
  vid_init();
  vid_clear();
  vid_puts("Hello, world\n");
  for(;;) {}
}
