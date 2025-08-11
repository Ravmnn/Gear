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


; int32 main() block...
main:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; int32 value = 10
    mov edi, 10
    sub rsp, 4
    mov [rsp], edi

    ; $value()
    mov edi, [rsp]
    call edi
    mov edi, eax

    ; return 0
    mov esi, 0
    mov eax, esi

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret





