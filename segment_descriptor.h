#ifndef SEGMENT_DESCRIPTOR_H
#define SEGMENT_DESCRIPTOR_H

// Types and functions for manipulating segment descriptors.
//
// See: Intel 64 and IA-32 Arthitectures Software Developer's Manual,
// System Programming Guide, section 3.4.5 (Segment Descriptors)

#include "segment_descriptor_bits.h"

// A segment descriptor.  You should treat this as a transparent type, using the
// functions in this module to access it.
typedef struct PACKED {
  uint16_t limit_low; // Segment limit 15:00
  uint16_t base_low;  // Base address 15:00
  uint8_t base_mid;   // Base address 23:16
  uint8_t access;     // Selector type
  uint8_t flags;      // Segment limit 19:16 + flags
  uint8_t base_high;  // Base address 31:24
} SegmentDescriptor;

// Initialize a segment descriptor.  Sets all fields to 0.
// Should be called first, before doing anything else with a segment
// descriptor.
void sd_init(SegmentDescriptor * sd);

// Set a segment descriptor's base address
void sd_set_address(SegmentDescriptor * sd, uint32_t address);

// Set a segment descriptor's limit.  Limit is in bytes if the
// segment's granularity is set to block granularity; pages otherwise.
void sd_set_limit(SegmentDescriptor * sd, uint32_t limit);

// Mark a segment desciptor as being present.
inline void sd_set_present(SegmentDescriptor * sd) {
  sd->access |= SD_ACCESS_PRESENT;
}

// Mark a segment as code.
inline void sd_set_code(SegmentDescriptor * sd) {
  sd->access |= SD_ACCESS_CODE;
}

// Mark a segment as data.
inline void sd_set_data(SegmentDescriptor * sd) {
  sd->access &= ~SD_ACCESS_CODE;
}

// Mark a segment as code or data (that is, not system).
inline void sd_set_code_or_data(SegmentDescriptor * sd) {
  sd->access |= SD_ACCESS_CODE_OR_DATA;
}

// Set a segment's privilege level.
inline void sd_set_priv(SegmentDescriptor * sd, int priv) {
  sd->access &= ~(SD_ACCESS_DPL_MASK << SD_ACCESS_LSBIT);
  sd->access |= (priv & SD_ACCESS_DPL_MASK) << SD_ACCESS_LSBIT;
}

// Set a segment's granularity to 1 (byte)
inline void sd_set_granularity_1(SegmentDescriptor * sd) {
  sd->flags &= ~SD_FLG_GRANULARITY_4K;
}

// Set a segment's granularity to 4k blocks
inline void sd_set_granularity_4k(SegmentDescriptor * sd) {
  sd->flags |= SD_FLG_GRANULARITY_4K;
}

// Set a segment's default size to 16-bit
inline void sd_set_default_size_16_bits(SegmentDescriptor * sd) {
  sd->flags &= ~SD_FLG_32BIT;
}

// Set a segment's default size to 32-bit
inline void sd_set_default_size_32_bits(SegmentDescriptor * sd) {
  sd->flags |= SD_FLG_32BIT;
}

// Set a data segment to writable
inline void sd_set_writable(SegmentDescriptor * sd) {
  sd->access |= SD_ACCESS_WRITABLE;
}

// Set a code segment to readable
inline void sd_set_readable(SegmentDescriptor * sd) {
  sd->access |= SD_ACCESS_READABLE;
}

#endif
