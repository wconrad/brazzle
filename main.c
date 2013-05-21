#include "bmmap.h"
#include "cpu.h"
#include "init.h"
#include "pmmap.h"
#include "vty.h"

void main() {
  init();
  vty_puts("Hello, world...\n");
  bmmap_print();
  pmmap_print();
  halt();
}
