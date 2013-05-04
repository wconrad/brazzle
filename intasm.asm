;;; intasm.asm

        bits    32

;;; Interrupt 3 (#BP - Breakpoint) stub.
;;; Calls the C handler.

        global  int3_stub
        extern  int3_handler
int3_stub:
        call    int3_handler
        iret

;;; Interrupt 8 (#DF - Double Fault) stub.
;;; Calls the C handler.

        global  int8_stub
        extern  int8_handler
int8_stub:
        call    int8_handler
        iret

;;; Interrupt 13 (#GP - General Protection) stub.
;;; Calls the C handler with all the registers as arguments
;;; On entry, the stack has:
;;;   eflags, cs, eip, error code

        global  int13_stub
        extern  int13_handler
int13_stub:
        pusha                   ; eax, ecx, edx, ebx, old esp, ebp, esi, edi
        push    ds
        push    ss
        push    es
        push    fs
        push    gs
        call    int13_handler
        pop     gs
        pop     fs
        pop     es
        pop     ss
        pop     ds
        popa
        add     sp,4            ; Get rid of error code
        iret

;;; Issue an int3

        global int3
int3:
        push    ebp
        mov     ebp,esp
        int     3
        pop     ebp
        ret

;;; Cause a GP fault

        global  gp_fault
gp_fault:
        mov     ebp,esp
        jmp     0xffff:0xffffffff
        pop     ebp
        ret
