;;; loader.asm

        %include 'mem.inc'

	bits    16

        section .text
        global  start
        extern  main
start: 

        ;; The boot sector will load us at 0:loader_addr
        ;; CS, DS and ES will all be 0
        
        jmp     0:start2

;;; Display NULL terminated string at ds:si
        
bios_print:
        lodsb
        or      al,al
        jz      .done
        mov     ah,0x0e
        mov     bx,7
        int     0x10
        jmp     bios_print
.done:
        ret
        
;;; Enable the A20 address line, so that all of memory can be
;;; addressed.
;;; 
;;; There are many ways of doing this, and none of them work on all
;;; systems throughout time.
;;; 
;;; See: "A20 Line":
;;;    http://wiki.osdev.org/A20_Line
;;; And: "A20 - a pain from the past":
;;;    http://www.win.tue.nl/~aeb/linux/kbd/A20.html
;;; 
;;; Mine is a modern system, and I don't really care about the past.
;;; Just get the BIOS to do it for us.

enable_a20:

	mov     ax,0x2401
        int     0x15
        jnc     .no_error
        mov     si,a20_fail_msg
        call    bios_print
        cli
        hlt
.no_error:
        ret

;;; Get the memory map using the BIOS.
;;; Stores it at bios_memmap
;;; See:
;;;   * http://wiki.osdev.org/Detecting_Memory_%28x86%29#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820

get_bios_memmap:                             
.magic: equ     'PAMS'                          ; 'MAPS'
        mov     edi,bios_memmap                 ; dest addr
        xor     ebx,ebx                         ; continuation value
        mov     dword [bios_memmap_entries],0
.again:
        mov     edx,.magic
        mov     eax,0xe820
        mov     ecx,bios_memmap_entry_size
        int     15h
        jc      .done
        cmp     eax,.magic
        jne     .done
        test    ebx,ebx
        jz      .done
        inc     dword [bios_memmap_entries]
        add     di,bios_memmap_entry_size
        cmp     dword [bios_memmap_entries],bios_memmap_max_entries
        jge     .done
        jmp     .again
.done:
        ret

;;; Strings
	
init_msg:       db      'Stage 2 starting',13,10,0
protmode_msg:   db      'Entering protected mode.',13,10,0
a20_msg:        db      'Enabling A20',13,10,0
bios_memmap_msg:db      'Getting memory map',13,10,0
a20_fail_msg:   db      'Failed to set A20',13,10,0

start2:

        ;; We're not prepared to handle interrupts, so disable them.
        ;; They should already be disabled, courtesy of the boot
        ;; sector, but let's make sure.

        cli

        ;; Display init message

        mov     si,init_msg
        call    bios_print
        
        ;; Turn on A20

	mov     si,a20_msg
        call    bios_print
        call    enable_a20

        ;; Get memory map

        mov     si,bios_memmap_msg
        call    bios_print
        call    get_bios_memmap

        ;; Display protected mode message

        mov     si,protmode_msg
        call    bios_print

        ;; Set the GDT

        lgdt    [gdt_descriptor]

        ;; Enter protected mode

        mov     eax,cr0
        or      al,1
        mov     cr0,eax

        ;; Clear cache and set CS to the code selector
        
        jmp     codesel:start_32bit
start_32bit:

	bits    32

        ;; Set data segments to data selector

        mov     ax,datasel
        mov     ds,ax
        mov     ss,ax
        mov     es,ax
        mov     esp,loader_tos
        
        ;; Modify video memory to indicate that we're now in protected
        ;; mode.

        mov     [0xb8000],dword 'P M '

        ;; call C main()

        call     main

        ;; main() should not return.  If it does, poke something into
        ;; video memory and halt.

        mov     [0xb8000],dword '( ) '
	cli
        hlt

;;; Selectors

nullsel:        equ     0x0000
codesel:        equ     0x0008
datasel:        equ     0x0010

;;; Export constants for C.  You can't export constants to C, but you
;;; can export variables with constant values.

        global  codesel_var
codesel_var:                 dw      codesel

        global  bios_memmap_entries_ptr
bios_memmap_entries_ptr:     dd      bios_memmap_entries

        global  bios_memmap_ptr
bios_memmap_ptr:             dd      bios_memmap

;;; The gtd descriptor points to, and gives the size of, the gdt.

gdt_descriptor:
        dw      gdt_size - 1
        dd      gdt

;;; Global descriptor table.

	align   8
gdt:

        ;; Entry 0 (select 0x0000): null descriptor
        dd      0
        dd      0

        ;; Entry 1 (selector 0x0008): code
        ;; Base 0, limit 0xffffffff, type 9a
        dw      0xffff          ; limit 0:15
        dw      0               ; base 0:15
        db      0               ; base 16:23
        db      0b10011010      ; access: present, ring 0, code, r
        db      0b11001111      ; flags: 4k blocks, 32-bit mode; limit 16:19
        db      0               ; base 24:31

        ;; Entry 2 (selector 0x0010): stack/data
        ;; base 0, limit 0xffffffff, type 92
        dw      0xffff          ; limit 0:15
        dw      0               ; base 0:15
        db      0               ; base 16:23
        db      0b10010010      ; access: present, ring 0, data, rw
        db      0b11001111      ; flags: 4k blocks, 32-bit mode; limit 16:19
        db      0               ; base 24:31

gdt_size:       equ     $ - gdt
