; Video driver code - switches the CPU back into real mode
; Then executes an int 0x10 instruction

%include "gdt.inc"

global do_vbe

bits 16
section .data.realmode
save_idt: dw 0
          dd 0
save_esp: dd 0
vid_mode: dw 0
real_ivt: dw (256 * 4) - 1      ; Realmode IVT has 256 CS:IP pairs
          dd 0                  ; Realmode IVT physical address at address 0x00000

align 4
mode_info:TIMES 129 dw 0        ; Buffer to store mode info from Int 10h/ax=4f01h
                                ; Plus additional bytes for the return status byte
                                ; at beginning of buffer

bits 32
section .text
do_vbe:
    mov ax, [esp+4]             ; Retrieve videomode passed on stack
    pushad                      ; Save all the registers
    pushfd                      ; Save the flags (including Interrupt flag)
    cli
    mov word [vid_mode],ax
    mov [save_esp],esp
    sidt [save_idt]
    lidt [real_ivt]             ; We use a real ivt that points to the
                                ; physical address 0x00000 at the bottom of
                                ; memory. The IVT in real mode is 256*4 bytes
                                ; and runs from physical address 0x00000 to
                                ; 0x00400
    jmp CODE16SEL:pmode16

bits 16
section .text.realmode
pmode16:
    mov ax,DATA16SEL
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov eax,cr0
    dec eax
    mov cr0,eax
    jmp 0:realmode1

realmode1:
    ; Sets real mode stack to grow down from 0x1000:0xFFFF
    mov ax,0x1000
    mov ss,ax
    xor sp,sp

    xor ax,ax
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax

    ; first zero out the 258 byte memory for the return function from getmodeinfo
    cld
    mov cx,(258/2)              ; 128 words + 1 word for the status return byte
    mov di,mode_info
    rep stosw

    mov ax,[vid_mode]
    xor ax,0x13
    jnz svga_mode

    ; Just a regular mode13
    mov ax,0x13
    int 0x10

    ; Fake a video mode structure with the stuff the kernel actually uses
    mov di, mode_info
    mov word [di+0x01],0xDD     ; mode attribs
    mov word [di+0x13],320      ; width
    mov word [di+0x15],200      ; height
    mov byte [di+0x1a],8        ; bpp
    mov byte [di+0x1c],1        ; memory model type = CGA
    mov dword [di+0x29],0xa0000 ; screen memory
    jmp done

svga_mode:
    mov ax,0x4f01               ; Get mode info function
    mov cx,[vid_mode]
    or cx,0x4000                ; always try to use linear buffer
    mov di,mode_info+0x01
    int 0x10
    mov [mode_info],ah
    or ah,ah
    jnz done

    mov ax,0x4f02               ; Now actually set the mode
    mov bx,[vid_mode]
    or bx,0x4000
    int 0x10

done:
    cli
    mov eax,cr0
    inc eax
    mov cr0,eax
    jmp dword CODE32SEL:pm1     ; To FAR JMP to address > 0xFFFF we need
                                ; to specify DWORD to allow a 32-bit address
                                ; in the offset portion. When this JMP is
                                ; complete CS will be CODE32SEL and processor
                                ; will be in 32-bit protected mode

bits 32
section .text
pm1:
    mov eax,DATA32SEL
    mov ds,ax
    mov es,ax
    mov fs,ax
    mov gs,ax
    mov ss,ax
    mov dword esp,[save_esp]
    lidt [save_idt]
    popfd                       ; Restore flags (including Interrupt flag)
    popad                       ; Restore registers

    mov eax, mode_info          ; Return pointer to mode_info structure
    ret