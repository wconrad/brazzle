; mem.inc - where to put things in memory

        
;;; Memory map.  BIOS Int 0x15, EAX=0xE820 memory map info is
;;; stored here while still in real mode, since you can't make BIOS calls
;;;  once in protected mode.
        
bios_memmap_entry_size:      equ     24
bios_memmap_max_entries:     equ     16
bios_memmap_entries:         equ     0x0500
bios_memmap:                 equ     0x0504

;;; stage2 loader and its stack
        
stage2_addr     equ     0x2000          ; Must match stage2.ld
stage2_tos      equ     stage2_addr
