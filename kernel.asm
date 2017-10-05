bits 32

section .text
    ;multiboot specifications i.e multiboot header
    align 4
    dd 0x1BADB002   ;magic field number
    dd 0x00         ;flags field
    dd - (0x1BADB002 + 0x00)    ;checksum m+f+c should be zero

global start
extern main_kernel

start:
    cli         ;block all interrupts
    mov esp, stack_space
    call main_kernel
    hlt

section .bss
resb 8192       ; 8KB for stack
stack_space:
