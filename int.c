#include "cpu.h"
#include "idt.h"
#include "int.h"
#include "pgm.h"
#include "vty.h"

// Stub for interrupt handlers.
// These are naked function which does not use the C calling sequence.
// They cannot be called from C (without crashing).  The IDT
// dispatches to them, and they call an int...handler function in return.

extern void int3_stub();
extern void int8_stub();
extern void int13_stub();

// Set the IDT register to the IDT.

static void
load_idt() {
  idt_addr_t idt_addr = {.addr = &idt, .size = sizeof(idt) - 1};
  lidt(&idt_addr);
}

// Int 3 (#BP - Breakpoint) handler.

void int3_handler() {
  vty_puts("int 3\n");
  halt();
}

// Int 8 (#DF - Double Fault) handler.

void int8_handler() {
  vty_puts("int 8\n");
  halt();
}

// Int 13 (#GP - General Protection) handler.

void int13_handler() {
  vty_puts("int 13\n");
  halt();
}

void int_init() {
  set_idt_entry(3, int3_stub, IDT_TRAP32);
  set_idt_entry(8, int8_stub, IDT_TRAP32);
  set_idt_entry(13, int13_stub, IDT_TRAP32);
  load_idt();
}
