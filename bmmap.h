#ifndef BMMAP_H
#define BMMAP_H

#include "nonstd.h"

// Values for bmmap_entry_t.type.  Any other types should be treated
// as "reserved"

#define BMMAP_TYPE_AVAILABLE 1
#define BMMAP_TYPE_RESERVED  2

// An entry in the BIOS memory map, as returned by BIOS INT 15h,
// AX=E820h

typedef struct PACKED bmmap_entry {
  unsigned base_addr_low;
  unsigned base_addr_high;
  unsigned length_low;
  unsigned length_high;
  unsigned type;
  unsigned reserved;
} bmmap_entry_t;

// Address of pointer to BIOS memory map.  Set by the stage 2 loader.

extern int * bios_memmap_entries_ptr;

// Address of memory map.  Set by the stage 2 loader.

extern bmmap_entry_t (*bios_memmap_ptr) [];


// Intialize.  Call this before using this module.

void bmmap_init();

// Print the BIOS memory map

void bmmap_print();

#endif
