#include "memmap.h"
#include "nonstd.h"
#include "vty.h"

typedef struct PACKED memmap_entry {
  unsigned base_addr_low;
  unsigned base_addr_high;
  unsigned length_low;
  unsigned length_high;
  unsigned type;
  unsigned reserved;
} memmap_entry_t;

extern int * memmap_entries_ptr;
extern memmap_entry_t (*memmap_ptr) [];

void print_bios_memmap() {
  vty_puts("BIOS memory map:\n");
  vty_printf("%d entries\n", *memmap_entries_ptr);
  vty_printf("memmap is at %p\n", memmap_ptr);
  for(int i = 0; i < *memmap_entries_ptr; i++) {
    memmap_entry_t * entry = &(*memmap_ptr)[i];
    vty_printf("%08x %08x %08x %08x %08x %08x\n",
               entry->base_addr_low,
               entry->base_addr_high,
               entry->length_low,
               entry->length_high,
               entry->type,
               entry->reserved);
  }
}
