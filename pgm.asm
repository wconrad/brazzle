; pgm.asm


;;; The boot sector will load us at CS:1000
;;; CS, DS and ES will be the same.
        
        org 1000h

;;; Display message

        mov     si,init_msg
        call    bios_print

;;; Turn on A20

        call    enable_a20

;;; Display message

        mov     si,init_done_msg
        call    bios_print

hang:   jmp     hang
        
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
.no_error:
        ret
a20_fail_msg:   db      'Failed to set A20',13,10,0

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

init_msg:       db      'Initializing',13,10,0
init_done_msg:  db      'Initialized',13,10,0
