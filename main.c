#include "vid.h"
#include "vty.h"

int main() {
  vty_init();
  vty_puts("Hello, world\n");
  for(;;) {}
}
