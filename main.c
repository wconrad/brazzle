#include "cpu.h"
#include "int.h"
#include "vty.h"

int main() {
  vty_init();
  int_init();
  vty_puts("Hello, world...\n");
  gp_fault();
  halt();
}
