#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  vty_printf("[%u]", 0);
  vty_printf("[%u]", 1234);
  vty_printf("[%u]", 0x7fffffff);
  vty_printf("[%u]", 0x80000000);
  vty_printf("[%u]", 0xffffffff);
  for(;;) {}
}
