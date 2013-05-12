;;; kinit.asm - Kernel initialization

        bits    32

        %include 'mem.inc'
        %include 'selectors.inc'

        section .text
        extern  main

;;; This is the beginning of the kernel.  The loader jumps here with these conditions:
;;; A20 is enabled, the CPU is in protected mode, and flat addressing is in place.
;;; CS is set to codesel; all other segment registers are set to datasel.
;;; Interrupts are disabled.
;;; The stack is at loader_tos.
;;; The BIOS memory map is at bios_memmap.

        ;; Call C main().

        call     main
 
        ;; main() should not return.  If it does, poke something into
        ;; video memory and halt.

        mov     [0xb8000],dword '( ) '
        cli
        hlt
        
;;; Export constants for C.  You can't export constants to C, but you
;;; can export variables with constant values.

        global  codesel_var
codesel_var:                 dw      codesel

        global  bios_memmap_entries_ptr
bios_memmap_entries_ptr:     dd      bios_memmap_entries

        global  bios_memmap_ptr
bios_memmap_ptr:             dd      bios_memmap
