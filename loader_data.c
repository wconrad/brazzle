#include "loader_data.h"

#include <stddef.h>

#include "string.h"

LoaderData loader_data;

// A pointer to the loader's copy of LoaderData.
extern LoaderData *loader_data_p;

void loader_data_init() {
  // Copy loader data from the loader's memory over to the kernel's.
  memmove(&loader_data, loader_data_p, sizeof(loader_data));
  // After soon as the loader's memory is reclaimed, loader_data_p
  // will be invalid, so set it to NULL now that we no longer need it.
  loader_data_p = NULL;  
}
