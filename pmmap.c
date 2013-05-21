#include "bmmap.h"
#include "pmmap.h"
#include "string.h"
#include "vty.h"

// Block size.
#define PMMAP_BLOCK_SIZE 0x1000

// Number of blocks of physical memory to map.
// = 4GB / PMMAP_BLOCK_SIZE
#define PMMAP_BLOCKS 0x00100000

// Bits per pmmap entry.
#define PMMAP_BITS 32

// The physical map size, in dwords.
#define PMMAP_SIZE (PMMAP_BLOCKS / PMMAP_BITS)

// Physical memory map.  This has one bit per block.  The first bit
// (bit 0) corresponds to physical address 0.  When a bit is clear,
// that block of memory is available.  When a bit is set, that block
// of memory is unavailable.  A block of memory is marked unavailable
// either when it has been allocated and is in use, or when it should
// never be allocated.
static uint32_t pmmap[PMMAP_SIZE];

// Return true if the block number is valid
static bool valid_block_number(int block_number) {
  return block_number >= 0 && block_number < PMMAP_BLOCKS;
}

// Given a block number, return the block's index in pmmap.
static int pmmap_index(int block_number) {
  return block_number / PMMAP_BITS;
}

// Given a block number, return the block's bit number in pmmap.
static int pmmap_bitnum(int block_number) {
  return block_number % PMMAP_BITS;
}

// Set (mark unavailable) a bit in the physical memory map.
// If the block number is out of range, does nothing.
static void pmmap_set(int block_number) {
  if(!valid_block_number(block_number))
    return;
  pmmap[pmmap_index(block_number)] |= (1 << pmmap_bitnum(block_number));  
}

// Reset (mark available) a bit in the physical memory map.  If the
// block number is out of range, does nothing.
static void pmmap_reset(int block_number) {
  if(!valid_block_number(block_number))
    return;
  pmmap[pmmap_index(block_number)] &= ~(1 << pmmap_bitnum(block_number));  
}

// Set (mark unavailable) or reset (mark available) a bit in the
// physical memory map.  If the block number is out of range, does
// nothing.
static void pmmap_set_value(int block_number, bool set) {
  if(set)
    pmmap_set(block_number);
  else
    pmmap_reset(block_number);
}

// Test if a bit is set (that is, the block is unavailable).  If the
// block number is out of range, return true (block unavailable).
static bool pmmap_test(int block_number) {
  if(!valid_block_number(block_number))
    return true;
  return pmmap[pmmap_index(block_number)] & (1 << pmmap_bitnum(block_number));  
}

// Return the size of the allocated or unallocated range starting at
// first_block_number.  If the block number is out of range,
// return 0.
static int pmmap_run_length(int first_block_number) {
  if(!valid_block_number(first_block_number))
    return 0;
  bool first_value = pmmap_test(first_block_number);
  int next_block_number = first_block_number + 1;
  while(next_block_number < PMMAP_BLOCKS &&
        pmmap_test(next_block_number) == first_value)
    next_block_number++;
  return next_block_number - first_block_number;
}

// Return the address of a block
static unsigned pmmap_addr(int block_number) {
  return PMMAP_BLOCK_SIZE * block_number;
}

// Return the number of bytes in a number of blocks
static unsigned pmmap_bytes(int num_blocks) {
  return PMMAP_BLOCK_SIZE * num_blocks;
}

// Set all bytes in the memory map
static void pmmap_memset(unsigned char value) {
  memset(pmmap, value, sizeof(pmmap));
}

void pmmap_mark_all_used() {
  pmmap_memset(0xff);
}

// Mark all memory as unused
static void pmmap_mark_all_unused() {
  pmmap_memset(0);
}

void pmmap_mark_region(PhysicalAddress addr,
                       unsigned length,
                       bool used) {
  uint32_t addr_value = (uint32_t) addr;
  int block_number = addr_value / PMMAP_BLOCK_SIZE;
  do {
    pmmap_set_value(block_number, used);
    block_number++;
  } while(pmmap_addr(block_number) < addr_value + length);
}

void pmmap_init() {
  pmmap_mark_all_unused();
}

void pmmap_print() {
  vty_puts("Physical memory map:\n");
  vty_puts("  BLK #   BLOCKS      ADDR     BYTES  USED\n");
  int block_number = 0;
  while(block_number < PMMAP_BLOCKS) {
    int run_length = pmmap_run_length(block_number);
    vty_printf("%7d  %7d  %08x  %08x  %d\n", block_number,
               run_length,
               pmmap_addr(block_number),
               pmmap_bytes(run_length),
               pmmap_test(block_number));
    block_number += run_length;
  }
}
