bits 32

section .text
    ;multiboot specifications i.e multiboot header
    align 4
    dd 0x1BADB002   ;magic field number
    dd 0x00         ;flags field
    dd - (0x1BADB002 + 0x00)    ;checksum m+f+c should be zero

global start
global keyboard_handler
global read_from_port
global write_to_port
global load_idt

extern main_kernel
extern keyboard_handler_main

read_from_port:
    mov edx, [esp+4]
    in al,dx
    ret
    
write_to_port:
    mov edx,[esp+4]
    mov al, [esp+4+4]
    out dx,al
    ret

load_idt:
    mov edx,[esp+4]
    lidt [edx]
    sti
    ret

keyboard_handler:
    call keyboard_handler_main
    iretd

start:
    cli         ;block all interrupts
    mov esp, stack_space
    call main_kernel
    hlt

section .bss
resb 8192       ; 8KB for stack
stack_space:
