#include "init.h"

#include "int.h"
#include "meminit.h"
#include "vty.h"

void init() {
  vty_init();
  int_init();
  mem_init();
}
