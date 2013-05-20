#include "init.h"

#include "bmmap.h"
#include "int.h"
#include "pmmap.h"
#include "vty.h"

void init() {
  vty_init();
  int_init();
  bmmap_init();
  pmmap_init();
}
