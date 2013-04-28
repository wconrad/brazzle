; pgm.asm

        [ORG 1000h]


;;; Write a string

        mov si, string

again:
        lodsb
        or  al,al
        jz  hang

        mov ah,0eh
        xor bh,bh
        int 10h
        
        jmp again

string: db 'Hello Cyberspace!',13,10,0

hang:                       ; Hang!
        jmp hang

times 510-($-$$) db 0
dw 0AA55h
