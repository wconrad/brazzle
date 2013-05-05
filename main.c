#include "cpu.h"
#include "int.h"
#include "bios_memmap.h"
#include "vty.h"

void main() {
  vty_init();
  int_init();
  vty_puts("Hello, world...\n");
  print_bios_memmap();
  halt();
}
