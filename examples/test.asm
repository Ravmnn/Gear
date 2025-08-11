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


    ; int32 value = $get()
    mov rdi, get
    call rdi
    sub rsp, 4
    mov [rsp], eax

    ; return 0
    mov eax, 0

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret

; int32 get() block...
get:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; return $get2
    mov rax, get2

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret

; int32 get2() block...
get2:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; return 10
    mov eax, 10

    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret

