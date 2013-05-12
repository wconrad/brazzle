;;; Boot sector
;;; 
;;; * Load program starting at sector 1
;;; * Run it

        %include 'mem.inc'

        bits    16

;;; The BIOS loads us at 0x00007c00, but with the segment register
;;; nknown (it could be 0000:7c00, or 007c0:0000, etc.)  Force
;;; the segment register to 0.

        org     boot_addr
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
        xor     ax,ax
        mov     ss,ax
        mov     sp,loader_tos
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

;;; Branch to it, but first load the boot drive back into dl, since the loader will need it.
;;; to load the kernel.

        mov     dl,[bootdrv]
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

;;; The boot drive which the BIOS passed to the boot sector in dl.
;;; This is given back to the BIOS when loading data from the disk.
        
bootdrv:        db      0

;;; Strings
        
bootmsg:        db      'Loading',13,10,0
loaded_msg:     db      'Loaded',13,10,0

;;; The disk address packet describes what to read and where to store
;;; it.

dap:    
        db      dap_size        ; size of packet
        db      0               ; always 0
        dw      loader_sectors  ; number of sectors to transfer
        dw      loader_addr     ; buffer offset
        dw      0               ; buffer segment
        dd      loader_lba      ; LBA (lower)
        dd      0               ; LBA (upper)
dap_size: equ $ - dap

        times 510-($-$$) db 0
        dw 0AA55h
