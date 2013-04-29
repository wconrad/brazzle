; pgm.asm

	bits    16

;;; The boot sector will load us at CS:1000
;;; CS, DS and ES will be the same.
        
        org     0x1000
        jmp     0:start
        
;;; Poke a char to the screen.  Simple debugging when nothing else
;;; works.
        
;;; in: al = character

poke_screen:
        push    ax
        push    ds
        push    ax
        mov     ax,0xb800
        mov     ds,ax
        pop     ax
        mov     ah,' '
        mov     word [0],ax
        pop     ds
        pop     ax
        ret

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

;;; Strings

init_msg:       db      'Initializing',13,10,0
protmode_msg:   db       'Entering protected mode.',13,10,0
a20_msg:        db      'Enabling A20',13,10,0
a20_fail_msg:   db      'Failed to set A20',13,10,0
	
start:

;;; Display message

        mov     si,init_msg
        call    bios_print
        
;;; Turn on A20

	mov     si,a20_msg
        call    bios_print
        call    enable_a20

;;; Display message

        mov     si,protmode_msg
        call    bios_print

;;; Set the GDT

        lgdt    [gdt_descriptor]

;;; Enter protected mode

        cli
        mov     eax,cr0
        or      al,1
        mov     cr0,eax

        jmp 0x0008:stage3

	bits    32
stage3:

;;; Set data segments to data selector

	cli
        mov     ax,0x10
        mov     ds,ax
        mov     ss,ax
        mov     es,ax
        mov     esp,0x9000
        sti
        
;;; Modify video memory

        mov     [0xb8000],dword 'P M '

;;; Halt

	cli
        hlt

	align   8
gdt_descriptor:
        dw      gdt_size - 1
        dd      gdt

	align   8
gdt:

        ;; Entry 0 (select 0x00): null descriptor
        dd      0
        dd      0

        ;; Entry 1 (selector 0x08): code
        ;; Base 0, limit 0xffffffff, type 9a
        dw      0xffff          ; limit 0:15
        dw      0               ; base 0:15
        db      0               ; base 16:23
        db      0b10011010      ; access: present, ring 0, code, r
        db      0b11001111      ; flags: 4k blocks, 32-bit mode; limit 16:19
        db      0               ; base 24:31

        ;; Entry 2 (selector 0x10): stack/data
        ;; base 0, limit 0xffffffff, type 92
        dw      0xffff          ; limit 0:15
        dw      0               ; base 0:15
        db      0               ; base 16:23
        db      0b10010010      ; access: present, ring 0, data, rw
        db      0b11001111      ; flags: 4k blocks, 32-bit mode; limit 16:19
        db      0               ; base 24:31
        
gdt_size:       equ     $ - gdt
