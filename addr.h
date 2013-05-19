#ifndef ADDR_H
#define ADDR_H

// Address manipulation

#include "inttypes.h"

// PDE (Page Directory Entry) bits

#define PDE_PRESENT (1 << 0)
#define PDE_WRITE (1 << 1)
#define PDE_USER (1 << 2)
#define PDE_WRITE_THROUGH (1 << 3)
#define PDE_NO_CACHE (1 << 4)
#define PDE_ACCESSED (1 << 5)
#define PDE_4MB (1 << 6)
#define PDE_FRAME 0xfffff000

// PTE (Page Directory Entry) bits

#define PTE_PRESENT (1 << 0)
#define PTE_WRITE (1 << 1)
#define PTE_USER (1 << 2)
#define PTE_WRITE_THROUGH (1 << 3)
#define PTE_NO_CACHE (1 << 4)
#define PTE_ACCESSED (1 << 5)
#define PTE_DIRTY (1 << 6)
#define PTE_PAT (1 << 7)
#define PTE_GLOBAL (1 << 8)
#define PTE_FRAME 0xfffff000

// A physical (linear) address.
typedef void * PhysicalAddress;

// A virtual address.
typedef void * virtual_address;

// A physical address that is not mapped by any virtual address.
#define PHYS_ADDR_NOT_MAPPED 0

// The size of a page table, in bytes
#define PT_BYTES 0x400

// Becasue the last entry of the page directory is mapped to itself, the
// last 4MB of virtual addresses refer to the page tables and the page
// table directory itself, at these addresses.

#define PAGE_TABLES_VIRT_ADDR 0xffc00000
#define PAGE_DIR_VIRT_ADDR 0xfffff000

// Extract the page directory index from a virtual address.
inline unsigned page_directory_index(virtual_address addr) {
  return (unsigned) addr >> 22;
}

// Extract the page table index from a virtual address.
inline unsigned page_table_index(virtual_address addr) {
  return (unsigned) addr >> 12 & 0x03FF;
}

// Extract the byte offset within a page from a virtual address.
inline unsigned page_offset(virtual_address addr) {
  return (unsigned) addr & 0xfff;
}

// Translate a virtual address into a physical address.
//
// Returns PHYS_ADDR_NOT_MAPPED if the virtual address is not mapped to a physical
// address.
//
// Depends upon the page table directory having its last
// entry mapped to itself.
//
// From: http://wiki.osdev.org/Paging
PhysicalAddress get_physaddr(virtual_address virt_addr);

#endif
