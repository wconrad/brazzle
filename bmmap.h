#ifndef BMMAP_H
#define BMMAP_H

#include <inttypes.h>
#include "nonstd.h"

// Values for BmmapEntry.type.  Any other types should be treated
// as "reserved"

#define BMMAP_TYPE_AVAILABLE 1
#define BMMAP_TYPE_RESERVED  2

// An entry in the BIOS memory map, as returned by BIOS INT 15h,
// AX=E820h

typedef struct PACKED bmmap_entry {
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
  uint32_t  reserved;
} BmmapEntry;

// Address of pointer to BIOS memory map.  Set by the stage 2 loader.

extern int32_t * bios_memmap_entries_ptr;

// Address of memory map.  Set by the stage 2 loader.

extern BmmapEntry (*bios_memmap_ptr) [];

// Intialize.  Call this before using this module.

void bmmap_init();

// Print the BIOS memory map

void bmmap_print();

#endif
