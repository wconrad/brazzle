#ifndef ADDR_H
#define ADDR_H

// A physical (linear) address.
typedef void * PhysicalAddress;

// A special phyiscal address representing "no memory"
#define NO_PHYS_MEM ((PhysicalAddress *) 0xffffffff)

// A virtual address.
typedef void * VirtualAddress;

#endif
