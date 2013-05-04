#include "cpu.h"
#include "idt.h"
#include "int.h"
#include "pgm.h"
#include "vty.h"

// Stub for the int3 handler.
// This is a naked function which does not use the C calling sequence.
// It cannot be called from C.  The IDT dispatches to it, and it
// calls int3_handler in return.

extern void int3_stub();

// Set the IDT register to the IDT.

static void
load_idt() {
  idt_addr_t idt_addr = {.addr = &idt, .size = sizeof(idt) - 1};
  lidt(&idt_addr);
}

// Int 3 (breakpoint) handler.

void int3_handler() {
  vty_puts("int 3\n");
  halt();
  vty_puts("int 3\n");
}

void int_init() {
  set_idt_entry(3, int3_stub, IDT_TRAP32);
  load_idt();
}
