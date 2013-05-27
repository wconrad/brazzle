#include "bmmap.h"

#include "loader_data.h"
#include "vty.h"

static const char * bmmap_type_str(const BiosMemoryMapEntry * entry) {
  switch(entry->type) {
  case BMMAP_TYPE_AVAILABLE:
    return "AVAIL";
  case BMMAP_TYPE_RESERVED:
    return "RESERVED";
  default:
    return "?";
  }
}

void bmmap_print() {                                            
  vty_puts("BIOS memory map:\n");
  vty_printf("       ADDR               LEN           TYPE    RESERVED\n");
  for(unsigned i = 0; i < loader_data.bios_memory_map.count; i++) {
    BiosMemoryMapEntry * entry = &loader_data.bios_memory_map.entries[i];
    vty_printf("%08x:%08x  %08x:%08x  %8s  %08x\n",
               entry->base_addr_high,
               entry->base_addr_low,
               entry->length_high,
               entry->length_low,
               bmmap_type_str(entry),
               entry->reserved);
  }
}
