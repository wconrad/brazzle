#ifndef LOADER_DATA_H
#define LOADER_DATA_H

// Loader data, initialized in the loader and then copied into the
// kernel.  The structures defined here must be compatible with (same
// size, etc.) those defined in loader.asm.

#include <inttypes.h>

#include "nonstd.h"

// Values for BmmapEntry.type.  Any other types should be treated
// as "reserved"

#define BMMAP_TYPE_AVAILABLE 1
#define BMMAP_TYPE_RESERVED  2

// An entry in the BIOS memory map, as returned by BIOS INT 15h,
// AX=E820h.

typedef struct PACKED {
  uint32_t base_addr_low;
  uint32_t base_addr_high;
  uint32_t length_low;
  uint32_t length_high;
  uint32_t type;
  uint32_t  reserved;
} BiosMemoryMapEntry;

// The size (number of entries) in the BiosMemoryMap.
#define BIOS_MEMORY_MAP_SIZE 24

// The BIOS memory map.
typedef struct PACKED {
  uint32_t count;
  BiosMemoryMapEntry entries[BIOS_MEMORY_MAP_SIZE];
} BiosMemoryMap;

// The loader data block.
typedef struct PACKED {
  BiosMemoryMap bios_memory_map;
} LoaderData;

// The loader data block.  This contains constants and data copied
// over from the loader.

extern LoaderData loader_data;

// Initialize loader_data, copying it from loader memory into kernel memory

extern void loader_data_init();

#endif
