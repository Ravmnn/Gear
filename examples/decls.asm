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


    ; int32 number1 = 53
    mov edi, 53
    sub rsp, 4
    mov [rsp], edi

    ; int32 number2 = 100
    mov edi, 100
    sub rsp, 4
    mov [rsp], edi

    ; int32 result = ((($number1 + $number2)) - ((10 - 5)))
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

    ;  block...
    ; start

    ; int32 inner2 = 7
    mov edi, 7
    sub rsp, 4
    mov [rsp], edi

    ; bool inner1 = true
    mov edi, 1
    sub rsp, 1
    mov [rsp], edi

    ; $inner1
    mov dil, [rsp]

    ; $inner2
    mov edi, [rsp + 1]

    add rsp, 17

    ; end

    ; return 0
    mov edi, 0

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret

; int32 test() block...
test:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; return (10 + 5)
    mov edi, 10
    mov esi, 5
    add edi, esi

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret

