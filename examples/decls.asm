bits 64


section .data



section .text

global _start
_start:
    mov rbp, rsp

    call main

    mov rax, 60
    mov rdi, 0
    syscall




main:
    mov edi, 8
    mov esi, 4
    sub edi, esi ; 8 - 4
    mov esi, 5
    mov edx, 9
    mov ecx, 3
    add edx, ecx ; 9 + 3
    add esi, edx ; 5 + (9 + 3)
    add edi, esi ; (8 - 4) + (5 + (9 + 3))
    ret
