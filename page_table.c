#include "page_table.h"

#include <inttypes.h>

extern inline unsigned page_directory_index(VirtualAddress addr);
extern inline unsigned page_table_index(VirtualAddress addr);
extern inline unsigned page_offset(VirtualAddress addr);

// Return a pointer to page table n (0 <= n < PT_ENTRIES)
static uint32_t *page_table(int n) {
  return ((uint32_t *) PAGE_TABLES_VIRT_ADDR) + (PT_ENTRIES * n);
}

// The loader mapped the last entry of the page directory to itself.
//
// This causes the last 4MB of virtual address space (addresses
// 0xffc00000 on) to be mapped into the page tables and the page table
// directory.
// 
// One level of indirection is thrown away, so that any address
// from 0xffc00000 through 0xffffefff are a page table, and
// 0xfffff000 through 0xfffffff are the page directory.
//
// From: http://wiki.osdev.org/Paging
PhysicalAddress get_physaddr(VirtualAddress virt_addr)
{
  unsigned pdindex = page_directory_index(virt_addr);
  unsigned ptindex = page_table_index(virt_addr);
  unsigned offset = page_offset(virt_addr);
  unsigned * pd = PAGE_DIR_VIRT_ADDR;
  if(!(pd[pdindex] & PDE_PRESENT))
     return PHYS_ADDR_NOT_MAPPED;
  uint32_t * pt = page_table(pdindex);
  if(!(pt[ptindex] & PTE_PRESENT))
     return PHYS_ADDR_NOT_MAPPED;
  uint32_t page_address = pt[ptindex] & PTE_FRAME;
  return (void *) (page_address + offset);
}

PhysicalAddress page_table_phys_addr(int n) {
  if (n < 0 || n >= PT_ENTRIES)
    return PHYS_ADDR_NOT_MAPPED;
  return get_physaddr(page_table(n));
}
