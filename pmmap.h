#ifndef PMMAP_H
#define PMMAP_H

#include <stdbool.h>

// Physical memory map.  Influenced by/cribbed from:
//   http://www.brokenthorn.com/Resources/OSDev17.html
// which is public domain.

// Initialize the physical memory map.  Must be called first.
void pmmap_init();

// Print the physical memory map
void pmmap_print();

// Mark all memory as used
void pmmap_mark_all_used();

// Mark a region of memory as available or not.
void pmmap_mark_region(unsigned addr,
                       unsigned length,
                       bool used);

#endif
