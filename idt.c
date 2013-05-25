#include "idt.h"

#include "gdt.h"
#include "nonstd.h"

IdtEntry idt[IDT_SIZE];

void
set_idt_entry(int interrupt_number,
              void (*stub)(),
              int flags) {
  IdtEntry * entry = idt + interrupt_number;
  entry->offset_low = (unsigned) stub & 0xffff;
  entry->code_selector = selector_for_index(CODESEL_INDEX);
  entry->unused = 0;
  entry->flags = flags;
  entry->offset_high = (unsigned) stub >> 16;
}
