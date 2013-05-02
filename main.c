#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  vty_printf("[%p]\n", &main);
  vty_printf("[%p]\n", (void *) 0);
  vty_printf("[%p]\n", (void *) 0xffffffff);
  vty_printf("[%12p]\n", (void *) 0xffffffff);
  vty_printf("[%-12p]\n", (void *) 0xffffffff);
  for(;;) {}
}
