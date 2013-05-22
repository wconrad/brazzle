#ifndef GDT_H
#define GDT_H

// Initialize and install a GDT (Global Descriptor Table).  The loader
// set up a temporary one for us, but we want to reclaim the loader's
// memory, so set it up again.
void gdt_init();

#endif
