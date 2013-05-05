;;; idtasm.asm

        bits    32

;;; extern void lidt(const idt_addr_t * idt_addr)

        global  lidt
lidt:
        push    ebp
        mov     ebp,esp
        push    esi
        mov     si,[ebp + 8]    ;idt_addr
        lidt    [si]
        pop     esi
        pop     ebp
        ret

;;; extern void sidt(const idt_addr_t * idt_addr);
  
        global  sidt
sidt:
        push    ebp
        mov     ebp,esp
        push    esi
        mov     si,[ebp + 8]    ;idt_addr
        sidt    [si]
        pop     esi
        pop     ebp
        ret
