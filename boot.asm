;;; Boot sector
;;; 
;;; * Load program starting at sector 1
;;; * Run it

        %include 'mem.inc'

        bits    16

;;; The BIOS loads us at 0x00007c00, but with the segment register
;;; nknown (it could be 0000:7c00, or 007c:0000, etc.)  Force
;;; the segment register to 0.

        org     0x7c00
        jmp     0:start
start:

;;; DS = ES = CS

        mov     ax,cs
        mov     ds,ax
        mov     es,ax

;;; Save the boot drive

        mov     [bootdrv],dl

;;; Set up a stack

        cli
        mov     ax,loader_tos
        mov     ss,ax
        mov     sp,0
        sti

;;; Display boot message

        mov     si,bootmsg
        call    message

;;; Read second stage loader from the drive using LBA addressing mode.

        mov     ah,0x42                 ; read sectors, LBA mode
        mov     dl,[bootdrv]            ; drive
        mov     si,dap                  ; disk address packet
        int     0x13

        mov     si,loaded_msg
        call    message

;;; Branch to it

        jmp     0:loader_addr

;;; Display NULL terminated string at ds:si
        
message:
        lodsb
        or      al,al
        jz      done
        call    bios_putc
        jmp     message
done:
        ret

;;; Call the BIOS to write AL to the screen, TTY style
        
bios_putc:      
        mov     ah,0x0e
        mov     bx,7                    ; attribute
        int     0x10
        ret

bootdrv:        db      0
bootmsg:        db      'Loading',13,10,0
loaded_msg:     db      'Loaded',13,10,0

;;; The disk address packet describes what to read and where to store
;;; it.

dap:    
        db      dap_size        ; size of packet
        db      0               ; always 0
        dw      10              ; number of sectors to transfer
        dw      loader_addr     ; buffer offset
        dw      0               ; buffer segment
        dd      1               ; LBA (lower)
        dd      0               ; LBA (upper)
dap_size: equ $ - dap

        times 510-($-$$) db 0
        dw 0AA55h
