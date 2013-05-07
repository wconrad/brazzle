#include "bmmap.h"
#include "cpu.h"
#include "int.h"
#include "pmmap.h"
#include "vty.h"

void main() {
  vty_init();
  int_init();
  bmmap_init();
  pmmap_init();
  vty_puts("Hello, world...\n");
  bmmap_print();
  pmmap_print();
  halt();
}
