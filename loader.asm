;;; loader.asm

        %include 'mem.inc'
        %include 'selectors.inc'

	bits    16

        section .text
        extern  main

        ;; The boot sector will load us at 0:loader_addr.
        ;; CS, DS and ES will all be 0.
        ;; The boot drive will be in DL.
        ;;
        ;; The far jump to force CS to 0 CS is not strictly necessary,
        ;; since the BIOS already did that.
        
        jmp     0:start

;;; Display NULL terminated string at ds:si using the BIOS.  Used for
;;; progress messages.
        
bios_print:
        lodsb
        or      al,al
        jz      .done
        mov     ah,0x0e
        mov     bx,7
        int     0x10
        jmp     bios_print
.done:
        ret
        
;;; Enable the A20 address line, so that all of memory can be
;;; addressed.
;;; 
;;; There are many ways of doing this, and none of them work on all
;;; systems throughout time.
;;; 
;;; See: "A20 Line":
;;;    http://wiki.osdev.org/A20_Line
;;; And: "A20 - a pain from the past":
;;;    http://www.win.tue.nl/~aeb/linux/kbd/A20.html
;;; 
;;; QEMU (and presumably, any reasonably modern system) has a BIOS
;;; that will do it for us, so let it do the work.

enable_a20:
	mov     ax,0x2401
        int     0x15
        jnc     .no_error
        mov     si,a20_fail_msg
        call    bios_print
        cli
        hlt
.no_error:
        ret

;;; Get the memory map using the BIOS.
;;; Stores it at bios_memmap
;;; See:
;;;   * http://wiki.osdev.org/Detecting_Memory_%28x86%29#BIOS_Function:_INT_0x15.2C_EAX_.3D_0xE820

get_bios_memmap:                             
.magic: equ     'PAMS'                          ; 'MAPS'
        mov     edi,bios_memory_map_entries     ; dest addr
        xor     ebx,ebx                         ; continuation value
        mov     dword [bios_memory_map_count],0
.again:
        mov     edx,.magic
        mov     eax,0xe820
        mov     ecx,bios_memmap_entry_size
        int     15h
        jc      .done
        cmp     eax,.magic
        jne     .done
        test    ebx,ebx
        jz      .done
        inc     dword [bios_memory_map_count]
        add     di,bios_memmap_entry_size
        cmp     dword [bios_memory_map_count],bios_memmap_max_entries
        jge     .done
        jmp     .again
.done:
        ret

;;;  Poke a char to the screen.  Simple debugging when nothing else
;;;  works.
;;;
;;;  in: al = character
	
poke_screen:
	push    ax
        push    ds
        push    ax
        mov     ax,0xb800
        mov     ds,ax
        pop     ax
        mov     ah,' '
        mov     word [0],ax
        pop     ds
        pop     ax
        ret

;;; Load the kernel from disk into low memory using the BIOS.

load_kernel:

        mov     ah,0x42                 ; read sectors, LBA mode
        mov     dl,[bootdrv]            ; drive
        mov     si,.dap                 ; disk address packet
        int     0x13
        ret

;;; The disk address packet describes what to read and where to store
;;; it.

.dap:    
        db      .dap_size               ; 0: size of packet
        db      0                       ; 1: always 0
        dw      kernel_sectors          ; 2: number of sectors to transfer
        dw      kernel_load_addr        ; 4: buffer offset
        dw      0                       ; 6: buffer segment
        dd      kernel_lba              ; 8: LBA (lower)
        dd      0                       ; 12: LBA (upper)
.dap_size: equ $ - .dap

;;; Strings
	
init_msg:       db      'Loader starting',13,10,0
protmode_msg:   db      'Entering protected mode.',13,10,0
a20_msg:        db      'Enabling A20',13,10,0
memmap_msg:     db      'Getting memory map',13,10,0
load_msg:       db      'Loading kernel',13,10,0
a20_fail_msg:   db      'Failed to set A20',13,10,0


;;; The boot drive which the boot sector passed to the loader in dl.
;;; This is given back to the BIOS when loading data from the disk.
        
bootdrv:        db      0

start:

        ;; We're not prepared to handle interrupts, so disable them.
        ;; They should already be disabled, courtesy of the boot
        ;; sector, but let's make sure.

        cli

        ;; Save the boot drive
        mov     [bootdrv],dl

        ;; The stack grows down from the loader.
        
        mov     esp,loader_tos

        ;; Display init message

        mov     si,init_msg
        call    bios_print
        
        ;; Turn on A20.  This allows the A20 address line to be used,
        ;; allowing all of memory to be addressed.

	mov     si,a20_msg
        call    bios_print
        call    enable_a20

        ;; Get memory map

        mov     si,memmap_msg
        call    bios_print
        call    get_bios_memmap

        ;; Load the kernel into low memory.  It won't yet be at its final address, so
        ;; we'll move it after switching to protected mode.

        mov     si,load_msg
        call    bios_print
        call    load_kernel

        ;; Enter protected mode
        ;; We set up a temporary GDT that the kernel will redefine
        ;; during its initialization

        mov     si,protmode_msg
        call    bios_print
        lgdt    [gdt_descriptor]
        mov     eax,cr0
        or      al,1
        mov     cr0,eax

        ;; Clear cache and set CS to the code selector.
        ;; This must come immediately after the "move cr0..." instruction,
        ;; according to Intel's documentation.
        
        jmp     codesel:start_32bit
start_32bit:
	bits    32

        ;; Set data segments to data selector

        mov     ax,datasel
        mov     ds,ax
        mov     ss,ax
        mov     es,ax
        mov     fs,ax
        
        ;; Modify video memory to indicate that we're now in protected
        ;; mode.  This won't last long enough to see it, if things go
        ;; well.

        mov     [0xb8000],dword 'P M '

        ;; Initialize paging

        call    init_paging

        ;; Copy the kernel from low memory to high memory

        call    move_kernel_to_high_memory

        ;; Start the kernel, passing it the address of the loader data
        ;; block

	lea     eax,[loader_data]
        jmp     kernel_virt_addr

;;; Initialize paging.
;;; Identity maps the first 4mb of memory.
;;; Maps virtual 0xc000_00000 to 0x10_0000 (1MB)

init_paging:
.priv:  equ     0b11            ; writable + present

        ;; Clear the page table.  A zero entry is not present.

        mov     edi,ptd
        xor     eax,eax
        mov     ecx,ptd_entries
        cld
        rep     stosd

        ;; Identity map page 0 (the first 4MB of memory)

        mov     eax,0           ; address of first page
        mov     edi,page_table_0
        call    init_page_table

        ;; Map the kernel's virtual virtual memory to its phsyical memory

        mov     eax,kernel_phys_addr
        mov     edi,page_table_kernel
        call    init_page_table

        ;; Point the PDT to the page tables

        mov     dword [ptd + 4 * 0],page_table_0 | .priv
        mov     dword [ptd + 4 * kernel_pdt_idx],page_table_kernel | .priv

        ;; Point the last PTE to the PDT itself.  This gives the PDT its own virtual
        ;; address, which is useful when translating virtual addresses to linear.

        mov     dword [ptd + 4 * 1023],ptd | .priv

        ;; Install the page table directory

        mov     eax,ptd
        mov     cr3,eax

        ;; Enable paging

        mov     eax,cr0
        or      eax,0x80000000
        mov     cr0,eax

        ret

;;; Initialize a page table to point to consecutive physical pages.
;;; in: di = address of page table
;;; in: ax = physical address of first page

init_page_table:
        or      eax,0b11        ; writable + present
        mov     ecx,1024        ; Number of entries in a page table
        cld
.again:
        stosd
        add     eax,0x1000      ; Next page
        loop    .again
        ret

;;; Copy the kernel from low memory to high memory

move_kernel_to_high_memory:
        mov     esi,kernel_load_addr
        mov     edi,kernel_virt_addr
        mov     ecx,kernel_max_bytes
        cld
        rep     movsb
        ret

;;; The gtd descriptor points to, and gives the size of, the gdt.

gdt_descriptor:
        dw      gdt_size - 1
        dd      gdt

;;; Global descriptor table.

	align   8
gdt:

        ;; Entry 0 (select 0x0000): null descriptor
        dd      0
        dd      0

        ;; Entry 1 (selector 0x0008): code
        ;; Base 0, limit 0xffffffff, type 9a
        dw      0xffff          ; limit 0:15
        dw      0               ; base 0:15
        db      0               ; base 16:23
        db      0b10011010      ; access: present, ring 0, code, r
        db      0b11001111      ; flags: 4k blocks, 32-bit mode; limit 16:19
        db      0               ; base 24:31

        ;; Entry 2 (selector 0x0010): stack/data
        ;; base 0, limit 0xffffffff, type 92
        dw      0xffff          ; limit 0:15
        dw      0               ; base 0:15
        db      0               ; base 16:23
        db      0b10010010      ; access: present, ring 0, data, rw
        db      0b11001111      ; flags: 4k blocks, 32-bit mode; limit 16:19
        db      0               ; base 24:31

gdt_size:       equ     $ - gdt

	section .bss

;;; The loader data block, which contains data that is passed to the
;;; kernel.  This must be consistent with the size and structure
;;; of the LoaderData struct defined in loader_data.h

bios_memmap_entry_size:         equ     24
bios_memmap_max_entries:        equ     16

loader_data:            equ     $
bios_memory_map:        equ     $
bios_memory_map_count:  resd    1
bios_memory_map_entries:resb    bios_memmap_max_entries * bios_memmap_entry_size
