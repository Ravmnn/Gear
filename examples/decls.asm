bits 64


section .data



section .text

global _start
_start:
    mov rbp, rsp ; init stack frame

    call main

    mov rax, 60 ; syscall exit
    mov rdi, 0
    syscall




; function main() -> int16
main:
    ; ((1 + 2) - ((1 + 1)))
    mov edi, 1
    mov esi, 2
    add edi, esi
    mov esi, 1
    mov edx, 1
    add esi, edx
    sub edi, esi

    ret

