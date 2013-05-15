// IDT (Interrupt Descriptor Table).
// The IDT is a data structure used by the processor
// to dispatch interrupts and exceptions.

#ifndef IDT_H
#define IDT_H

#include "nonstd.h"

// IDT flags.  Or these together.

// Bit 7 is the "present" bit.
#define IDT_PRESENT (1 << 7)

// Bits 6..5 are the privilege level
#define IDT_RING0 (0 << 5)
#define IDT_RING1 (1 << 5)
#define IDT_RING2 (2 << 5)
#define IDT_RING3 (3 << 5)

// Bits 4..0 are the gate type

#define IDT_TASK_GATE   0x05
#define IDT_INT16_GATE  0x06
#define IDT_INT32_GATE  0xe0
#define IDT_TRAP16_GATE 0x07
#define IDT_TRAP32_GATE 0x0f

// Composite IDT flags

#define IDT_TRAP32 (IDT_PRESENT | IDT_RING0 | IDT_TRAP32_GATE)

// An IDT entry.  This directs an interrupt to a task gate, interrupt
// gate, or trap gate.

typedef struct PACKED idt_entry {
  short offset_low;
  short code_selector;
  char unused;
  unsigned char flags;
  short offset_high;
} idt_entry_t;

// The IDT's linear address and size.
// The size is special: It's actually the size - 1.
// addr must be the _linear_ (not segmented, not virtual) address of
// the IDT
typedef struct PACKED idt_addr {
  unsigned short size;
  void * addr;
} idt_addr_t;

// The number of entries in the IDT (1..256)

#define IDT_SIZE 17

// The interrupt descriptor table (IDT).

extern idt_entry_t idt[IDT_SIZE];

// Load the IDT register

extern void
lidt(const idt_addr_t * idt_addr);

// Store the IDT register

extern void
sidt(idt_addr_t * idt_addr);

// Set an IDT entry.
// interrupt_number is 0...IDT_SIZE
// Stub should be the address of a stub such as int3_stub, defined
// in assembly and specifically designed to accept the interrupt.
// It must not be a C function.
// flags is IDT_TRAP32, etc.

void 
set_idt_entry(int interrupt_number,
              void (*stub)(),
              int flags);

#endif
