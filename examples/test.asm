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



; int32 main()block...
main:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; int32 value = 10
    mov edi, 10
    sub rsp, 4
    mov [rsp], edi

    ; int8 newValue = ($value as int8)
    mov edi, [rsp] ; value
    mov sil, dil
    sub rsp, 1
    mov [rsp], sil

    ; int64 result = ($value + ((($newValue + $get())) as int32))
    mov edi, [rsp + 1] ; value
    mov sil, [rsp] ; newValue
    mov rdx, get ; get
    call rdx
    mov dl, al
    add sil, dl
    movzx edx, sil
    add edi, edx
    sub rsp, 8
    mov [rsp], edi

    ; int8 smallResult = ($result as int8)
    mov rdi, [rsp] ; result
    mov sil, dil
    sub rsp, 1
    mov [rsp], sil

    ; block...
    ; start

    ; int8 inner = 1
    mov edi, 1
    sub rsp, 1
    mov [rsp], edi

    ; int64 innera = 1
    mov edi, 1
    sub rsp, 8
    mov [rsp], edi

    add rsp, 9

    ; end

    ; $smallResult
    mov dil, [rsp] ; smallResult


    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret





; int8 get()block...
get:
    ; stack frame begin
    push rbp
    mov rbp, rsp
    add rbp, 16


    ; return 3
    mov edi, 3
    mov eax, edi


    ; stack frame end
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    ret





