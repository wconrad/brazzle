;;; Boot sector
;;; 
;;; * Load program starting at sector 1
;;; * Switch to protected mode
;;; * Run it

;;; Tell the compiler that this is offset 0.
;;; It isn't offset 0, but it will be after the jump.

        bits    16
        org     0x7c00

        jmp     0:start         ;cs = 0
start:

;;; DS = ES = CS

        mov     ax,cs
        mov     ds,ax
        mov     es,ax

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

;;; Read second stage loader from the drive.

        mov     ah,2            ; read sectors from drive
        mov     al,10           ; sector count
        mov     ch,0            ; track
        mov     cl,2            ; sector
        mov     dh,0            ; head
        mov     dl,[bootdrv]    ; drive
        mov     bx,0x1000
        int     0x13

        mov     si,loaded_msg
        call    message

;;; Branch to it

        jmp     0:0x1000

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
loaded_msg:     db      'Loaded',13,10,0

        times 510-($-$$) db 0
dw 0AA55h
