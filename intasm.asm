;;; intasm.asm

        bits    32

;;; Push registers that the handler will want to see.

%macro pushall 0
        pusha                   ; eax, ecx, edx, ebx, old esp, ebp, esi, edi
        push    ds
        push    ss
        push    es
        push    fs
        push    gs
%endmacro

;;; Pop registers pushed by the pushall macro

%macro popall 0
        pop     gs
        pop     fs
        pop     es
        pop     ss
        pop     ds
        popa
%endmacro

;;; Load many registers with constant values.  Used during debugging
;;; to show that the registers were correctly passed to the C handler.

%macro set_registers_for_debugging 0
        mov     ax,0x0010
        mov     fs,ax
        mov     gs,ax
        mov     eax,0x11111111
        mov     ebx,0x22222222
        mov     ecx,0x33333333
        mov     edx,0x44444444
        mov     ebp,0x55555555
        mov     esi,0x66666666
        mov     edi,0x77777777
%endmacro

;;; Define an interrupt stub.  An IDT entry directs an interrupt to its stub;
;;; the stub puts registers on the stack, so they can be parameters to the C
;;; handler function, and then calls the handler.
;;;
;;; On entry to the stub, the CPU has pushed onto the stack:
;;;   eflags cs eip error_code
;;;
;;; Macro parameters: The interrupt number (3, 13, etc.)

%macro int_stub 1
        global  int%1_stub
        extern  int%1_handler
int%1_stub:
        pushall
        call    int%1_handler
        popall
        add     sp,4            ; Get rid of error code
        iret
%endmacro

;;; Interrupt stubs.

        int_stub 3              ; #BP - Breakpoint
        int_stub 8              ; #DF - Double Fault
        int_stub 13             ; #GP - General Protection

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
