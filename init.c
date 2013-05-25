#include "init.h"

#include "gdt.h"
#include "int.h"
#include "loader_data.h"
#include "meminit.h"
#include "vty.h"

void init() {
  loader_data_init();
  vty_init();
  gdt_init();
  int_init();
  mem_init();
}
