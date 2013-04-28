; pgm.asm


;;; The boot sector will load us at CS:1000
;;; CS, DS and ES will be the same.
        
        org 1000h

;;; Write a string

        mov si, string

again:
        lodsb
        or  al,al
hang:   
        jz  hang

        mov ah,0eh
        xor bx,bx
        int 10h
        
        jmp again

string: db 'Hello Cyberspace!',13,10,0
