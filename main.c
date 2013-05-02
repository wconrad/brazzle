#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

static void print_d(int n) {
  char buffer[ITODEC_BUFF_LEN_DEC];
  vty_printf("[%s]\n", itodec(buffer, n));
}

static void print_u(unsigned n) {
  char buffer[ITODEC_BUFF_LEN_DEC];
  vty_printf("[%s]\n", utodec(buffer, n));
}

static void print_h(unsigned n) {
  char buffer[UTODEC_BUFF_LEN_HEX];
  vty_printf("[%s]\n", utohex(buffer, n));
}

static void print_n(int n) {
  print_d(n);
  print_u(n);
  print_h(n);
}

int main() {
  vty_init();
  vty_puts("Hello, world...\n");
  print_n(0x80000000);
  print_n(-123);
  print_n(0x00000000);
  print_n(+123);
  print_n(0x7fffffff);
  for(;;) {}
}
