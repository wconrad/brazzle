#include "int.h"

#include <inttypes.h>

#include "page_table.h"
#include "cpu.h"
#include "idt.h"
#include "nonstd.h"
#include "vty.h"

// Stub for interrupt handlers.
// These are naked function which does not use the C calling sequence.
// They cannot be called from C (without crashing).  The IDT
// dispatches to them, and they call an int...handler function in return.

extern void int3_stub();
extern void int8_stub();
extern void int13_stub();

// The registers pushed onto the stack by the CPU before calling the
// stub.

typedef struct PACKED {
  unsigned eip;
  unsigned cs;
  unsigned eflags;
} TrapRegisters;

// The registers pushed onto the stack by the stub.

typedef struct PACKED {
  unsigned gs;
  unsigned fs;
  unsigned es;
  unsigned ss;
  unsigned ds;
  unsigned edi;
  unsigned esi;
  unsigned ebp;
  unsigned esp;
  unsigned ebx;
  unsigned edx;
  unsigned ecx;
  unsigned eax;
} StubRegisters;

// Parameters passed to a handler which receives an error code.

typedef struct PACKED {
  StubRegisters stub_registers;
  unsigned error_code;
  TrapRegisters trap_registers;
} TrapParams;

// Parameters passed to a handler which receives an error code.

typedef struct PACKED {
  StubRegisters stub_registers;
  unsigned error_code;
  TrapRegisters trap_registers;
} TrapParamsWithError;

static void print_registers(const StubRegisters * stub_registers,
                            const TrapRegisters * trap_registers) {
  vty_printf(" eflags=%08x", trap_registers->eflags);
  vty_puts("\n");
  vty_printf(" eax=%08x", stub_registers->eax);
  vty_printf(" ebx=%08x", stub_registers->ebx);
  vty_printf(" ecx=%08x", stub_registers->ecx);
  vty_printf(" edx=%08x", stub_registers->edx);
  vty_puts("\n");
  vty_printf(" ebp=%08x", stub_registers->ebp);
  vty_printf(" esi=%08x", stub_registers->esi);
  vty_printf(" edi=%08x", stub_registers->edi);
  vty_puts("\n");
  vty_printf(" cs:eip=%04x:%08x", trap_registers->cs, trap_registers->eip);
  vty_printf(" ss:esp=%04x:%08x", stub_registers->ss, stub_registers->esp);
  vty_puts("\n");
  vty_printf(" ds=%04x", stub_registers->ds);
  vty_printf(" es=%04x", stub_registers->es);
  vty_printf(" fs=%04x", stub_registers->fs);
  vty_printf(" gs=%04x", stub_registers->gs);
  vty_puts("\n");
}

// Print information for a trap without an error code.
static void print_trap(const char * name,
                TrapParams * params) {
  vty_printf("\n%s fault\n", name);
  print_registers(&params->stub_registers, &params->trap_registers);
}

// Print information for a trap with an error code.
static void print_trap_with_error(const char * name,
                           TrapParamsWithError * params) {
  vty_printf("\n%s fault %04x\n", name, params->error_code);
  print_registers(&params->stub_registers, &params->trap_registers);
}

// Int 3 (#BP - Breakpoint) handler.
void int3_handler(TrapParams params) {
  print_trap("BP", &params);
  halt();
}

// Int 8 (#DF - Double Fault) handler.
void int8_handler(TrapParamsWithError params) {
  print_trap_with_error("DF", &params);
  halt();
}

// Int 13 (#GP - General Protection) handler.
void int13_handler(TrapParamsWithError params) {
  print_trap_with_error("GP", &params);
  halt();
}

// Set the IDT register to the IDT.

static void load_idt() {
  IdtAddr idt_addr = {
    .addr = get_physaddr(idt),
    .size = sizeof(idt) - 1,
  };
  lidt(&idt_addr);
}

void int_init() {
  set_idt_entry(3, int3_stub, IDT_TRAP32);
  set_idt_entry(8, int8_stub, IDT_TRAP32);
  set_idt_entry(13, int13_stub, IDT_TRAP32);
  load_idt();
}
