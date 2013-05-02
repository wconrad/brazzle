#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

static void decode(const char * s) {
  const char * endptr;
  unsigned n = dectou(s, &endptr);
  vty_printf("%d\n", n);
  vty_printf("[%s] [%s] [%u]\n", s, endptr, n);
}

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  /* vty_printf("[%u]", 123); */
  /* vty_printf("[%3u]", 1234); */
  /* vty_printf("[%4u]", 1234); */
  /* vty_printf("[%5u]", 1234); */
  /* vty_printf("[%05u]", 1234); */
  decode("");
  decode("0");
  decode("123");
  decode("123x");
  for(;;) {}
}
