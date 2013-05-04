;;; intasm.asm

        bits    32

;;; Interrupt 3 (#BP - Breakpoint) stub.
;;; Calls the C handler.

        global  int3_stub
        extern  int3_handler
int3_stub:
        call    int3_handler
        iret

;;; Interrupt 8 (#DF - Double fault) stub.
;;; Calls the C handler.

        global  int8_stub
        extern  int8_handler
int8_stub:
        call    int8_handler
        iret

;;; Interrupt 13 (#GP - General Protection Exception) stub.
;;; Calls the C handler.

        global  int13_stub
        extern  int13_handler
int13_stub:
        call    int13_handler
        iret

;;; Issue an int3

        global int3
int3:
        push    ebp
        mov     ebp,esp
        int     3
        pop     ebp
        ret
