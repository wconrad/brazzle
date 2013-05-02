#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  vty_printf("[%s]", "abc");
  vty_printf("[%u]", 123);
  vty_printf("[%d]", -123);
  vty_printf("[%%]");
  vty_printf("[%?]");
  vty_printf("%");
  vty_printf("\n");
  vty_printf("[%3u]", 1234);
  vty_printf("[%4u]", 1234);
  vty_printf("[%5u]", 1234);
  vty_printf("[%05u]", 1234);
  vty_printf("[%10s]", "abc");
  vty_printf("[%-10s]", "abc");
  for(;;) {}
}
