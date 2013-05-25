#include "gdt.h"

#include <inttypes.h>

#include "nonstd.h"
#include "page_table.h"
#include "segment_descriptor.h"

// GDT indices

#define CODESEL_INDEX 1
#define DATASEL_INDEX 2

// The global descriptor table (GDT)
static SegmentDescriptor gdt[3] ALIGNED8;

// The GDT's linear address and size.
// The size is special: It's actually the size - 1.
// addr must be the _linear_ (not segmented, not virtual) address of
// the GDT
typedef struct PACKED {
  uint16_t size;
  uint32_t addr;
} GdtAddr;

// Load the GDT register.  Has no effect until the segment registers
// are set.
extern void lgdt(const GdtAddr * gdt_addr);

// Store the GDT register.
extern void sgdt(const GdtAddr * gdt_addr);

// Set the code (CS) selector.
extern void set_code_selector(uint32_t selector);

// Set the data selectors (DS, ES, FS, GS) and stack selector (SS)
extern void set_data_selectors(uint32_t selector);

// Setup most of a segment descriptor.  Segment discriptors only in
// only a few details.
static void mostly_set_descriptor(SegmentDescriptor * sd) {
  sd_set_address(sd, 0);
  sd_set_limit(sd, 0xffffff);
  sd_set_present(sd);
  sd_set_priv(sd, 0);
  sd_set_default_size_32_bits(sd);
  sd_set_granularity_4k(sd);
  sd_set_code_or_data(sd);
}

// Initialize and install the GDT to replace the temporary GDT set up
// by the loader.
static void gdt_set() {
  // Clear all GDT entries.  We'll leave entry 0 (select 0x0000, the
  // NULL selector) empty,
  // as it needs nothing in it--Segment 0 has special meaning to the processor.
  for(unsigned i = 0; i < sizeof(gdt) / sizeof(*gdt); i++)
    sd_init(gdt + i);
  { // entry 1 (selector 0x0008): code
    SegmentDescriptor * sd = gdt + CODESEL_INDEX;
    mostly_set_descriptor(sd);
    sd_set_code(sd);
    sd_set_readable(sd);
  }
  { // entry 2 (selector 0x0010): data
    SegmentDescriptor * sd = gdt + DATASEL_INDEX;
    mostly_set_descriptor(sd);
    sd_set_data(sd);
    sd_set_writable(sd);
  }
}

// Given a GDT index, return the selector
static inline uint32_t selector_for_index(unsigned index) {
  return index * 0x0008;
}

// Install the GDT so that the processor will use it.
static void gdt_install() {
  GdtAddr gdt_addr = {
    .addr = (uint32_t) get_physaddr(gdt),
    .size = sizeof(gdt) - 1,
  };
  lgdt(&gdt_addr);
  set_code_selector(selector_for_index(CODESEL_INDEX));
  set_data_selectors(selector_for_index(DATASEL_INDEX));
}

void gdt_init() {
  gdt_set();
  gdt_install();
}
