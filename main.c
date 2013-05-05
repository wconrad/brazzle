#include "cpu.h"
#include "int.h"
#include "bios_memmap.h"
#include "vty.h"

void main() {
  vty_init();
  int_init();
  vty_puts("Hello, world...\n");
  vty_printf("[%032b]\n", 0x12345678);
  print_bios_memmap();
  halt();
}
