#include "addr.h"
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
  vty_printf("%p\n", get_physaddr((void *) 0xc0000000)); // kernel
  vty_printf("%p\n", get_physaddr((void *) 0x000b8000)); // video memory
  vty_printf("%p\n", get_physaddr((void *) 0xffc00000)); // ptd
  vty_printf("%p\n", get_physaddr((void *) 0x01000000)); // missing memory
  halt();
}
