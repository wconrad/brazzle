#ifndef PMMAP_H
#define PMMAP_H

// Physical memory map.  Influenced by/cribbed from:
//   http://www.brokenthorn.com/Resources/OSDev17.html
// which is public domain.

// Initialize the physical memory map.  Must be called first.
void pmmap_init();

// Print the physical memory map
void pmmap_print();

#endif
