;;; Boot sector
;;; 
;;; * Load program starting at sector 1
;;; * Switch to protected mode
;;; * Run it

;;; Tell the compiler that this is offset 0.
;;; It isn't offset 0, but it will be after the jump.

        org 0
        jmp 07C0h:start     ; Goto segment 07C0

start:

;;; DS = ES = CS

        mov ax,cs
        mov ds,ax
        mov es,ax

;;; Save the boot drive

        mov     [bootdrv],dl

;;; Set up a stack

        cli
        mov     ax,0x900
        mov     ss,ax
        mov     sp,0
        sti

;;; Display boot message

        mov     si,bootmsg
        call    message

;;; Read sector from the drive

        mov     ah,2            ; read sectors from drive
        mov     al,10           ; sector count
        mov     ch,0            ; track
        mov     cl,2            ; sector
        mov     dh,0            ; head
        mov     dl,[bootdrv]    ; drive
        mov     bx,0x1000
        int     0x13

;;; Branch to it

        jmp     0x1000

;;; Display NULL terminated string at ds:si
        
message:
        lodsb
        or      al,al
        jz      done
        mov     ah,0x0e
        mov     bx,7
        int     0x10
        jmp     message
done:
        ret
        

bootdrv:        db      0
bootmsg:        db      'Loading',13,10,0
a20_fail_msg:   db      'Failed to enable A20',13,10,0
pmode_msg:      db      'Entering protected mode',13,10,0
foo_msg:        db      'foo',13,10,0

;;; Here's the locations of my IDT and GDT.  Remember, Intel's are
;;; little endian processors, therefore, these are in reversed order.
;;; Also note that lidt and lgdt accept a 32-bit address and 16-bit
;;; limit, therefore, these are 48-bit variables.
        
pIDT:
        dw 7FFh ; limit of 256 IDT slots
        dd 0000h ; starting at 0000
        
pGDT:
        dw 17FFh ; limit of 768 GDT slots
        dd 0800h ; starting at 0800h (after IDT)
        
times 510-($-$$) db 0
dw 0AA55h
