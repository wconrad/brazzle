;;; stringasm.asm

        bits    32

;;; unsigned memset(void * s, int c, unsigned length)

        global  memset
memset:
.s:             equ     8 + 4 * 0
.c:             equ     8 + 4 * 1
.length:        equ     8 + 4 * 2
        push    ebp
        mov     ebp,esp
        mov     ecx,[ebp+.length]
        or      ecx,ecx
        jz      .done
        mov     al,[ebp+.c]
        mov     edi,[ebp+.s]
        cld
        rep stosb
.done:
        pop     ebp
        ret
