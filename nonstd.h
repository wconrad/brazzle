#ifndef NONSTD_H
#define NONSTD_H

// Macros for non-standard compiler extensions.

// Prohibit the compiler from adding any padding to a struct.
// Some structs have to be laid out _just so_, because, for example,
// the processor makes use of them directly.
//
// Use:
//   typedef PACKED {...} foo;
#define PACKED __attribute__ ((packed))

// Align a variable on an 8-byte boundary.
//
// use:
//   IdtEntry idt[32] ALIGNED8;
#define ALIGNED8 __attribute__ ((aligned (8)))

#endif
