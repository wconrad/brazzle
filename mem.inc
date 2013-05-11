;;; mem.inc - where are things in memory?
;;;
;;; See: http://wiki.osdev.org/Memory_Map_%28x86%29
;;;
;;; In real mode, 1MB of memory is addressable.  Here's the initial
;;; memory map in real mode, when the BIOS loads and runs the boot
;;; sector.
;;;
;;; 0x00000-0x9ffff - RAM, initially used as follows:
;;;   0x00000-0x003ff - IVT (Interrupt vector table)
;;;   0x00400-0x004ff - BDA (BIOS data area)
;;;   0x00500-0x07bff - Avaialble
;;;   0x07c00-0x07dff - Boot sector
;;;   0x07e00-0x7ffff - Available
;;;   0x80000-0x9fbff - Available, _if it exists_
;;;   0x9fc00-0x9ffff - EBDA (Extended BIOS Data Area)
;;; 0xa0000-0xfffff - ROMS, memory-mapped I/O, etc.:
;;;   0xa0000-0xbffff - video RAM
;;;   0xc0000-0xc7fff - Video BIOS
;;;   0xc8000-0xeffff - ROMs, mapped hardware, etc.
;;;   0xf0000-0xfffff - BIOS
;;;
;;; What's in the memory from 0xa0000-0xffff doesn't change, but how
;;; the RAM from 0x00000-0x9ffff is used does change.  This list is
;;; just how the RAM is being used when the BIOS loads the boot sector
;;; and jumps to it.
        
;;; Memory map.  BIOS Int 0x15, EAX=0xE820 memory map info is stored
;;; here while still in real mode, since you can't make BIOS calls
;;; once in protected mode.
        
bios_memmap_entry_size:      equ     24
bios_memmap_max_entries:     equ     16
bios_memmap_entries:         equ     0x0500
bios_memmap:                 equ     0x0504

;;; loader and its stack
        
loader_addr     equ     0x2000          ; Must match loader.ld
loader_tos      equ     loader_addr
loader_sectors  equ     10              ; Number of sectors used by loader

;;; Kernel load address.  The loader, when still in real mode, uses
;;; the BIOS to load the kernel here.  The kernel may extend up to
;;; 0x9ffff, and so may be as large as 0x9d000 (643,027) bytes.

kernel_load_addr     equ 0x3000

;;; Boot sector, loaded by the BIOS.

boot_addr        equ    0x7c00

;;; Kernel executation address.  After the loader switches to
;;; protected mode and enables page mapping, it moves the kernel to
;;; this virtual address.

kernel_addr          equ 0xc000000
