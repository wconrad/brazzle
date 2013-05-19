#include "idt.h"
#include "loader.h"
#include "nonstd.h"

IdtEntry idt[IDT_SIZE];

void
set_idt_entry(int interrupt_number,
              void (*stub)(),
              int flags) {
  IdtEntry * entry = idt + interrupt_number;
  entry->offset_low = (unsigned) stub & 0xffff;
  entry->code_selector = codesel_var;
  entry->unused = 0;
  entry->flags = flags;
  entry->offset_high = (unsigned) stub >> 16;
}
