// Interrupt handling

#ifndef INT_H
#define INT_H

// Initialize this module.  Must be called first.

void int_init();

// Cause an INT 3 (breakpoint)

extern void int3();

#endif
