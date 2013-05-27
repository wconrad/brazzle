#ifndef PMMAP_H
#define PMMAP_H

// Physical memory map.  Influenced by/cribbed from:
//   http://www.brokenthorn.com/Resources/OSDev17.html
// which is public domain.

#include <stdbool.h>

#include "addr.h"

// Block size (in bytes)
#define PMMAP_BLOCK_SIZE 0x1000

// Initialize the physical memory map.  Must be called first.
void pmmap_init();

// Print the physical memory map
void pmmap_print();

// Mark all memory as used
void pmmap_mark_all_used();

typedef enum {
  BLOCK_AVAIL,
  BLOCK_UNAVAIL,
} BlockStatus;

// Mark a region of memory as available or not.
// If the range encompasses partial blocks, the entire blocks are
// marked.
void pmmap_mark_region(PhysicalAddress addr,
                       unsigned length,
                       BlockStatus status);

#endif
