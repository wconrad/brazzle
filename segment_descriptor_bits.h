#ifndef SEGMENT_DESCRIPTOR_BITS_H
#define SEGMENT_DESCRIPTOR_BITS_H

// Segment descriptor bits.  Only the segment_descriptor module should
// need to use these.

#include <inttypes.h>

//----- SegmentDescriptor access bits

// Segment is present
#define SD_ACCESS_PRESENT (1 << 7)

// DPL (Descriptor Privilege Level)
#define SD_ACCESS_LSBIT 5
#define SD_ACCESS_DPL_0 (0 << LSBIT)
#define SD_ACCESS_DPL_1 (1 << LSBIT)
#define SD_ACCESS_DPL_2 (2 << LSBIT)
#define SD_ACCESS_DPL_3 (3 << LSBIT)
#define SD_ACCESS_DPL_MASK 0x03

// S - Descriptor type (0 = system, 1 = code or data)
#define SD_ACCESS_CODE_OR_DATA (1 << 4)

// C - Code (0 = data, 1 = code)
#define SD_ACCESS_CODE (1 << 3)

// EC - If data, expand down; if code, conforming
#define SD_ACCESS_EXPAND_DOWN (1 << 2)
#define SD_ACCESS_CONFORMING (1 << 2)

// WR - If data, writable; if code, readable
#define SD_ACCESS_WRITABLE (1 << 1)
#define SD_ACCESS_READABLE (1 << 1)

// A - Accessed
#define SD_ACCESS_ACCESSED (1 << 1)

//----- SegmentDescriptor flag bits

// Flag "granularity" bit: If 0, limit is in byte blocks.  If 1, limit
// is in 4k blocks.
#define SD_FLG_GRANULARITY_4K (1 << 7)

// Flag "size" bit: If 0, selector defines 16-bit protected mode.  If
// 1, defines 32-bit protected mode.
#define SD_FLG_32BIT (1 << 6)

// Mask for accessing the "limit 19:36" part of the flags byte
#define SD_FLG_LIMIT_MASK 0x0f

#endif
