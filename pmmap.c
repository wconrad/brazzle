#include "bmmap.h"
#include "pmmap.h"
#include "string.h"
#include "vty.h"

// Number of blocks of physical memory to map.
// = 4GB / PMMAP_BLOCK_SIZE
#define PMMAP_BLOCKS 0x00100000

// Bits per pmmap entry = number of bits in a uint32_t.
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

// Set the status of a memory block.  If the block number is out of
// range, does nothing.  If BlockStatus is unknown, also does nothing.
static void pmmap_mark_block(int block_number, BlockStatus block_status) {
  switch(block_status) {
  case BLOCK_AVAIL:
    pmmap_reset(block_number);
    break;
  case BLOCK_UNAVAIL:
    pmmap_set(block_number);
    break;
  }
}

// Test whether or not a block is available.  If the
// block number is out of range, return BLOCK_UNAVAIL.
static BlockStatus pmmap_get_block_status(int block_number) {
  if(!valid_block_number(block_number))
    return true;
  if (pmmap[pmmap_index(block_number)] & (1 << pmmap_bitnum(block_number)))
    return BLOCK_UNAVAIL;
  else
    return BLOCK_AVAIL;
}

// Return the size of the allocated or unallocated range starting at
// first_block_number.  If the block number is out of range,
// return 0.
static int pmmap_run_length(int first_block_number) {
  if(!valid_block_number(first_block_number))
    return 0;
  BlockStatus first_status = pmmap_get_block_status(first_block_number);
  int next_block_number = first_block_number + 1;
  while(next_block_number < PMMAP_BLOCKS &&
        pmmap_get_block_status(next_block_number) == first_status)
    next_block_number++;
  return next_block_number - first_block_number;
}

// Return the address of a block
static PhysicalAddress pmmap_addr(int block_number) {
  return (PhysicalAddress) (PMMAP_BLOCK_SIZE * block_number);
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

// Translate a BlockStatus into a string
static const char * pmmap_status_str(BlockStatus status) {
  switch(status) {
  case BLOCK_AVAIL:
    return "AVAIL";
  case BLOCK_UNAVAIL:
    return "UNAVAIL";
  default:
    return "?";
  }
}

void pmmap_mark_region(PhysicalAddress addr,
                       unsigned length,
                       BlockStatus status) {
  uint32_t addr_value = (uint32_t) addr;
  int block_number = addr_value / PMMAP_BLOCK_SIZE;
  do {
    pmmap_mark_block(block_number, status);
    block_number++;
  } while(pmmap_addr(block_number) < (PhysicalAddress) (addr_value + length));
}

void pmmap_init() {
  pmmap_mark_all_unused();
}

void pmmap_print() {
  vty_puts("Physical memory map:\n");
  vty_puts("  BLK #   BLOCKS      ADDR     BYTES  STATUS\n");
  int block_number = 0;
  while(block_number < PMMAP_BLOCKS) {
    int run_length = pmmap_run_length(block_number);
    vty_printf("%7d  %7d  %08x  %08x  %s\n", block_number,
               run_length,
               pmmap_addr(block_number),
               pmmap_bytes(run_length),
               pmmap_status_str(pmmap_get_block_status(block_number)));
    block_number += run_length;
  }
}
