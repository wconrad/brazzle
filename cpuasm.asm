;;; cpuasm.asm

        bits    32

;;; Halt the processor

        global  halt
halt:
        push    ebp
        mov     ebp,esp
        hlt
        pop     ebp
        ret
