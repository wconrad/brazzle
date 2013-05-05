#include "idt.h"
#include "stage2.h"

idt_entry_t idt[IDT_SIZE];

void
set_idt_entry(int interrupt_number,
              void (*stub)(),
              int flags) {
  idt_entry_t * entry = idt + interrupt_number;
  entry->offset_low = (unsigned) stub & 0xffff;
  entry->code_selector = codesel_var;
  entry->unused = 0;
  entry->flags = flags;
  entry->offset_high = (unsigned) &stub >> 16;
}
