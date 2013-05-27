#include "meminit.h"

#include <inttypes.h>
#include <stdbool.h>

#include "loader_data.h"
#include "page_table.h"
#include "pmmap.h"

// The physical address of the kernel, and its size in bytes.

extern uint32_t kernel_phys_addr_var;
extern uint32_t kernel_max_bytes_var;

// Apply BIOS memory map entries of a certain availability.
static void apply_bios_map_if(bool avail) {
  for(unsigned i = 0; i < loader_data.bios_memory_map.count; i++) {
    BiosMemoryMapEntry * entry = loader_data.bios_memory_map.entries + i;
    if(avail != (entry->type == BMMAP_TYPE_AVAILABLE))
      continue;
    // We're not prepared to handle big memory (> 4GB)
    if(entry->base_addr_high != 0 || entry->length_high != 0)
      continue;
    pmmap_mark_region((PhysicalAddress) entry->base_addr_low,
                      entry->length_low,
                      avail ? BLOCK_AVAIL : BLOCK_UNAVAIL);
  }
}

// Apply the BIOS memory map to the physical memory map.
// This marks as available all the memory that the physical memory map
// shows as being available.
//
// This handles the following odd situations that the BIOS memory map
// could have:
// * Regions that does not start on a block boundary
// * Regions the size of which are not a multiple of the block size
// * Overlapping regions with either the same type or different types
// * Regions with unknown types
// Whenever there is a doubt, a block of memory is marked used
// (unavailable).
static void apply_bios_map() {
  // Assume not available
  pmmap_mark_all_used();
  // Unless the BIOS says it's available
  apply_bios_map_if(true);
  // Unless the BIOS says it's not available
  apply_bios_map_if(false);
}

// Mark as used the memory in which the kernel sits.
static void mark_kernel_memory_used() {
  pmmap_mark_region((PhysicalAddress) kernel_phys_addr_var,
                    kernel_max_bytes_var,
                    BLOCK_UNAVAIL);
}

// Mark as used the pages used by the page directory and page tables
static void mark_page_tables_used() {
  for(int i = 0; i < PT_ENTRIES; i++) {
    PhysicalAddress phys_addr = page_table_phys_addr(i);
    if(phys_addr != PHYS_ADDR_NOT_MAPPED)
      pmmap_mark_region(phys_addr, PT_BYTES, BLOCK_UNAVAIL);
  }
}

void mem_init() {
  pmmap_init();
  apply_bios_map();
  mark_kernel_memory_used();
  mark_page_tables_used();
}
