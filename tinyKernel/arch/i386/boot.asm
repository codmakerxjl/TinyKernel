%include "gdt.inc"

STACKSIZE equ 0x4000

bits 32
global mbentry

extern kmain
extern kernel_early_main
; Multiboot Header
section .multiboot
MBALIGN     equ 1<<0
MEMINFO     equ 1<<1
VIDINFO     equ 0<<2
FLAGS       equ MBALIGN | MEMINFO | VIDINFO
MAGIC       equ 0x1BADB002
CHECKSUM    equ -(MAGIC + FLAGS)

mb_hdr:
    dd MAGIC
    dd FLAGS
    dd CHECKSUM

section .text
mbentry:
    cli
    cld
    mov esp, stack_top

    ; EAX = magic number. Should be 0x2badb002
    ; EBX = pointer to multiboot_info
    ; Pass as parameters right to left
    push eax
    push ebx
    call kernel_early_main
    call kmain

    ; Infinite loop to end program
    cli
endloop:
    hlt
    jmp endloop

section .bss
align 32
stack:
    resb STACKSIZE
stack_top: