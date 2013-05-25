#ifndef GDT_H
#define GDT_H

#include <inttypes.h>

// GDT indices
#define CODESEL_INDEX 1
#define DATASEL_INDEX 2

// Given a GDT index, return the selector
inline uint32_t selector_for_index(unsigned index) {
  return index * 0x0008;
}

// Initialize and install a GDT (Global Descriptor Table).  The loader
// set up a temporary one for us, but we want to reclaim the loader's
// memory, so set it up again.
void gdt_init();

#endif
