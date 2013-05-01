#include "conv.h"
#include "sprintf.h"
#include "string.h"
#include "vty.h"

static void print_d(int n) {
  char buffer[ITOA_BUFF_LEN];
  vty_printf("[%s]\n", itoa(buffer, n));
}

static void print_u(unsigned n) {
  char buffer[ITOA_BUFF_LEN];
  vty_printf("[%s]\n", utoa(buffer, n));
}

static void print_h(unsigned n) {
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
