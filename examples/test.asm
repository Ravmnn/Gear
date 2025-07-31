bits 64


section .text

global _start
_start:
    ; 2 + 5 - 1

    mov edi, 2
    mov esi, 5
    add edi, esi

    mov esi, 1
    sub edi, esi


    mov rax, 60
    mov rdi, 0
    syscall