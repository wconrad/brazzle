#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  vty_printf("[%x]", 0x1234);
  vty_printf("[%8x]", 0x1234);
  vty_printf("[%08x]", 0x1234);
  for(;;) {}
}
