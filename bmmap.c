#include "bmmap.h"
#include "vty.h"

void bmmap_init() {
}

void bmmap_print() {                                            
  vty_puts("BIOS memory map:\n");
  vty_printf("%d entries at %p\n",
             *bios_memmap_entries_ptr,
             bios_memmap_ptr);
  vty_printf("ADDR LOW   ADDR HI   LEN LOW    LEN HI      TYPE  RESERVED\n");
  for(int i = 0; i < *bios_memmap_entries_ptr; i++) {
    bmmap_entry_t * entry = &(*bios_memmap_ptr)[i];
    vty_printf("%08x  %08x  %08x  %08x  %08x  %08x\n",
               entry->base_addr_low,
               entry->base_addr_high,
               entry->length_low,
               entry->length_high,
               entry->type,
               entry->reserved);
  }
}
