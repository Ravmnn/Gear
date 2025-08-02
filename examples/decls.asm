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
    ret

