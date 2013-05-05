#include "cpu.h"
#include "int.h"
#include "vty.h"

void main() {
  vty_init();
  int_init();
  vty_puts("Hello, world...\n");
  halt();
}
