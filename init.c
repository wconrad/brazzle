#include "init.h"

#include "gdt.h"
#include "int.h"
#include "meminit.h"
#include "vty.h"

void init() {
  vty_init();
  int_init();
  gdt_init();
  mem_init();
}
