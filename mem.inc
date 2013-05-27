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
;;;   0x80000-0x9ffff - EBDA
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

;;; The EBDA is special, because it is in use even after switching to
;;; protected mode and should never be overwritten.  It is always
;;; before 0xa0000 and is guaranteed to be no more than 0x20000 (128k)
;;; bytes long.  In practice, it is usually much smaller (with QEMU it
;;; is 4k).  We'll just give it all the berth it might need.
        
ebda_start:     equ     0x80000
	
;;; The loader and its stack.  The BIOS needs plenty of stack; perhaps
;;; as much as 0x1000 bytes.
	
loader_addr     equ     0x2000          ; Must match loader.ld
loader_tos      equ     loader_addr     ; Grows down
loader_sectors  equ     10              ; Number of sectors used by loader

;;; The LBA of the loader on disk.  Must agree with the Makefile
;;; commands to build hd.img
	
loader_lba:     equ     1

;;; The page table directory.  Uses 0x1000 bytes; must be 4k aligned.

ptd:            equ     0x3000
ptd_entries:    equ     1024

;;; Page table 0.  Uses 0x1000 bytes; must be 4k aligned.

page_table_0:   equ     0x4000

;;; Kernel page table.  Used 0x1000 bytes; must be 4k aligned.

page_table_kernel:      equ     0x5000

;;; Kernel load address.  The loader, when still in real mode, uses
;;; the BIOS to load the kernel here.  Must be on a segment boundary.

kernel_load_addr:       equ     0x6000  ; Must match kernel.ld
kernel_max_addr:        equ     ebda_start
kernel_max_bytes:       equ     kernel_max_addr - kernel_load_addr
kernel_sectors:         equ     kernel_max_bytes / 512

;;; The LBA of the kernel on disk.  Must agree with the Makefile
;;; commands to build hd.img
                        
kernel_lba:     equ     100

;;; Boot sector, loaded by the BIOS.

boot_addr:      equ     0x7c00

;;; Kernel physical address.  The loader maps virtual address
;;; kernel_addr to this physical address.
;;; Must be on a page (4k) boundary.

kernel_phys_addr:       equ     0x100000

;;; Kernel virtual address.  After the loader switches to protected
;;; mode and enables page mapping, it moves the kernel to this virtual
;;; address.
;;;
;;; Must be on a 4MB boundary, because it must be mapped by a single
;;; page table, and each page table maps 4MB.
                        
kernel_virt_addr:       equ     0xc0000000

;;; The index into page table directory of the kernel page table.
        
kernel_pdt_idx: equ     kernel_virt_addr / 0x400000
