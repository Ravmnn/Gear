bits 64


section .data



section .text

global _start
_start:
    mov rbp, rsp ; init stack frame

    call main

    ; syscall exit
    mov rax, 60
    mov rdi, 0
    syscall




; function main() -> int32
main:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; number1: int32 = 53
    mov edi, 53
    sub rsp, 4
    mov [rsp], edi

    ; number2: int32 = 100
    mov edi, 100
    sub rsp, 4
    mov [rsp], edi

    ; result: int32 = ((($number1 + $number2)) - ((10 - 5)))
    mov edi, [rsp + 4]
    mov esi, [rsp]
    add edi, esi
    mov esi, 10
    mov edx, 5
    sub esi, edx
    sub edi, esi
    sub rsp, 4
    mov [rsp], edi

    ; $result
    mov edi, [rsp]

    ; return 0
    mov edi, 0

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret

