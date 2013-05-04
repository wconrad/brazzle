;;; intasm.asm

        bits    32

;;; Interrupt 3 (breakpoint) handler.  For now, just poke the screen
;;; and halt.

        global  int3_stub
        extern  int3_handler
int3_stub:
        call    int3_handler
        ;; mov     [0xb8000],dword '* * '
        hlt

;;; Issue an int3

        global int3
int3:
        push    ebp
        mov     ebp,esp
        int     3
        pop     ebp
        ret
