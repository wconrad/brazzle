; 1.ASM
; Load a sector and run it

; Tell the compiler that this is offset 0.
; It isn't offset 0, but it will be after the jump.

        org 0
        jmp 07C0h:start     ; Goto segment 07C0

start:
        ; Update the segment registers
        mov ax, cs
        mov ds, ax
        mov es, ax

;;; Save the boot drive

        mov     [bootdrv],dl

;;; Read sector from the drive

        mov     ah,2            ; read sectors from drive
        mov     al,1            ; sector count
        mov     ch,0            ; track
        mov     cl,2            ; sector
        mov     dh,0            ; head
        mov     dl,[bootdrv]    ; drive
        mov     bx,1000h
        int     13h

;;; Branch to it

        jmp     1000h

bootdrv:        db      0
        
times 510-($-$$) db 0
dw 0AA55h
