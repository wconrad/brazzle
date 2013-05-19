#include "addr.h"
#include <inttypes.h>

extern inline unsigned page_directory_index(virtual_address addr);
extern inline unsigned page_table_index(virtual_address addr);
extern inline unsigned page_offset(virtual_address addr);

// The loader mapped the last entry of the page directory to itself.
//
// This causes the last 4MB of virtual address space (addresses
// 0xffc00000 on) to be mapped into the page tables and the page table
// directory.
// 
// One level of indirection is thrown away, so that any address
// from 0xffc00000 through 0xffffefff is into a page table, and
// 0xfffff000 through 0xfffffff is into the page table directory
// itself. //

PhysicalAddress get_physaddr(virtual_address virt_addr)
{
  unsigned pdindex = page_directory_index(virt_addr);
  unsigned ptindex = page_table_index(virt_addr);
  unsigned offset = page_offset(virt_addr);
  unsigned * pd = (unsigned *) PAGE_DIR_VIRT_ADDR;
  if(!(pd[pdindex] & PDE_PRESENT))
     return PHYS_ADDR_NOT_MAPPED;
  uint32_t * pt = ((uint32_t *) PAGE_TABLES_VIRT_ADDR) + (PT_BYTES * pdindex);
  if(!(pt[ptindex] & PTE_PRESENT))
     return PHYS_ADDR_NOT_MAPPED;
  uint32_t page_address = pt[ptindex] & PTE_FRAME;
  return (void *) (page_address + offset);
}
