;;; start.asm - Kernel start

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
;;; The address of the loader data block is in eax

        ;; Store the loader data block address for later use.

        mov     [loader_data_p],eax

        ;; Call C main(), passing it the address of the loader data
        ;; block.

        call    main
 
        ;; main() should not return.  If it does, poke something into
        ;; video memory and halt.

        mov     [0xb8000],dword '( ) '
        cli
        hlt

	section         .bss
        
;;; Export constants for C.  You can't export constants to C, but you
;;; can export variables with constant values.

        global  codesel_var
codesel_var:                    dw      codesel

        global  kernel_phys_addr_var
kernel_phys_addr_var:           dd      kernel_phys_addr

        global  kernel_max_bytes_var
kernel_max_bytes_var:           dd      kernel_max_bytes

;;; The address of the loader data block
        global  loader_data_p
loader_data_p:          resd    1
