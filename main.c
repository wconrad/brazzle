#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  char buffer[32];
  sprintf(buffer, sizeof(buffer), "Foo\n");
  vty_puts(buffer);
  sprintf(buffer, sizeof(buffer), "1234567890123456789012345678901234567890123456789012345678901234567890\n");
  vty_puts(buffer);
  vty_puts("\n");
  vty_printf("Bar\n");
  vty_printf("1234567890123456789012345678901234567890123456789012345678901234567890\n");
  vty_printf("100%%\n");
  vty_printf("%\n");
  vty_printf("%%\n");
  vty_printf("[%d]\n", 0x80000000);
  vty_printf("[%d]\n", -123);
  vty_printf("[%d]\n", 0);
  vty_printf("[%d]\n", 123);
  vty_printf("[%d]\n", 0x7fffffff);
  vty_printf(strrev("")); vty_printf("\n");
  vty_printf(strrev("a")); vty_printf("\n");
  vty_printf(strrev("ab")); vty_printf("\n");
  vty_printf(strrev("abc")); vty_printf("\n");

  itoa(buffer, 0x80000000);
  vty_printf(buffer);
  vty_printf("\n");

  itoa(buffer, 0);
  vty_printf(buffer);
  vty_printf("\n");

  itoa(buffer, 0x7fffffff);
  vty_printf(buffer);
  vty_printf("\n");

  for(;;) {}
}
