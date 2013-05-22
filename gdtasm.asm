;;; gdtasm.asm

        bits    32

;;; extern void lgdt(const GdtAddr * gdt_addr)

        global  lgdt
lgdt:
        push    ebp
        mov     ebp,esp
        push    esi
        mov     si,[ebp + 8]    ;gdt_addr
        lgdt    [si]
        pop     esi
        pop     ebp
        ret

;;; extern void set_code_selector(uint32_t selector)
        
        global  set_code_selector
set_code_selector:
        push    ebp
        mov     ebp,esp
        push    dword [ebp + 8] ;selector
        push    dword set_cs    ;offset
        retf                    ;far jump to set_csy
set_cs: 
        pop     ebp
        ret        

;;; extern void set_data_selectors(uint32_t selector)
        
        global  set_data_selectors
set_data_selectors:
        push    ebp
        mov     ebp,esp
        mov     eax,[ebp + 8]   ;selector
        mov     ds,eax
        mov     es,eax
        mov     fs,eax
        mov     gs,eax
        mov     ss,eax
        pop     ebp
        ret        

;;; extern void sgdt(const GdtAddr * gdt_addr);
  
        global  sgdt
sgdt:
        push    ebp
        mov     ebp,esp
        push    esi
        mov     si,[ebp + 8]    ;gdt_addr
        sgdt    [si]
        pop     esi
        pop     ebp
        ret
