#ifndef PAGE_TABLE_H
#define PAGE_TABLE_H

#include "addr.h"

// Page directory and page table manipulation

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

// The size of a page table, in dword entries
#define PT_ENTRIES 0x400

// The size of a page table, in bytes
#define PT_BYTES (PT_ENTRIES * sizeof(uint32_t))

// Because the last entry of the page directory is mapped to itself, the
// last 4MB of virtual addresses refer to the page tables and the page
// table directory itself, at these addresses.
#define PAGE_TABLES_VIRT_ADDR ((VirtualAddress) 0xffc00000)
#define PAGE_DIR_VIRT_ADDR ((VirtualAddress) 0xfffff000)

// A physical address that is not mapped by any virtual address.
#define PHYS_ADDR_NOT_MAPPED 0

// Extract the page directory index from a virtual address.
inline unsigned page_directory_index(VirtualAddress addr) {
  return (unsigned) addr >> 22;
}

// Extract the page table index from a virtual address.
inline unsigned page_table_index(VirtualAddress addr) {
  return (unsigned) addr >> 12 & 0x03FF;
}

// Extract the byte offset within a page from a virtual address.
inline unsigned page_offset(VirtualAddress addr) {
  return (unsigned) addr & 0xfff;
}

// Translate a virtual address into a physical address.
//
// Returns PHYS_ADDR_NOT_MAPPED if the virtual address is not mapped to a physical
// address.
PhysicalAddress get_physaddr(VirtualAddress virt_addr);

// Return the physical address of a page table or page directory.
// if 0 <= n < PT_ENTRIES - 1, returns the address of a page table
// If n == PT_ENTRIES - 1, returns the address of the page directory.
// If not mapped, or if n is out or range, returns PHYS_ADDR_NOT_MAPPED.
PhysicalAddress page_table_phys_addr(int n);

#endif
