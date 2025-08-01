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
    ; (((((((((((((100 + ((50 - ((30 + ((20 - 10)))))))) - ((((15 - 5)) + ((25 - 10)))))) + ((((40 + 60)) - ((10 - ((5 + 5)))))))) - ((((((20 - 10)) + ((15 - ((5 + 5)))))) - ((5 + ((5 - 2)))))))) + ((((((((30 - 10)) + ((25 - ((15 + 5)))))) - ((((20 + 10)) - ((5 - 3)))))) + ((((10 - ((5 + 3)))) + ((((15 + 5)) - ((5 + 2)))))))))) - ((((((((((50 + 60)) - ((((30 - 20)) + ((10 - 5)))))) + ((((40 - 25)) + ((35 - 15)))))) - ((((((20 + 10)) - ((5 + 5)))) + ((((15 - 5)) - ((10 + 5)))))))) + ((((((((25 + 35)) - ((15 - 10)))) + ((((20 - 10)) + ((15 - 5)))))) - ((((((10 + 10)) - ((5 - 2)))) + ((((5 + 5)) - ((3 + 1)))))))))))) + ((((((((((((100 - 50)) + ((75 - 25)))) - ((((60 + 40)) - ((30 - 20)))))) + ((((((45 + 35)) - ((25 + 15)))) + ((((20 - 10)) - ((10 - 5)))))))) - ((((((((15 + 15)) - ((10 + 5)))) + ((((5 + 5)) - ((3 + 2)))))) - ((((((30 - 20)) + ((25 - 15)))) - ((((10 + 5)) + ((5 - 3)))))))))) + ((((((((40 + 60)) - ((((30 - 20)) + ((10 - 5)))))) + ((((25 - 15)) + ((20 - 10)))))) - ((((((15 + 10)) - ((5 + 5)))) + ((((10 - 5)) - ((5 + 2)))))))))))
    mov edi, 20
    mov esi, 10
    sub edi, esi
    mov esi, 30
    add esi, edi
    mov edi, 50
    sub edi, esi
    mov esi, 100
    add esi, edi
    mov edi, 15
    mov edx, 5
    sub edi, edx
    mov edx, 25
    mov ecx, 10
    sub edx, ecx
    add edi, edx
    sub esi, edi
    mov edi, 40
    mov edx, 60
    add edi, edx
    mov edx, 5
    mov ecx, 5
    add edx, ecx
    mov ecx, 10
    sub ecx, edx
    sub edi, ecx
    add esi, edi
    mov edi, 20
    mov edx, 10
    sub edi, edx
    mov edx, 5
    mov ecx, 5
    add edx, ecx
    mov ecx, 15
    sub ecx, edx
    add edi, ecx
    mov edx, 5
    mov ecx, 2
    sub edx, ecx
    mov ecx, 5
    add ecx, edx
    sub edi, ecx
    sub esi, edi
    mov edi, 30
    mov edx, 10
    sub edi, edx
    mov edx, 15
    mov ecx, 5
    add edx, ecx
    mov ecx, 25
    sub ecx, edx
    add edi, ecx
    mov edx, 20
    mov ecx, 10
    add edx, ecx
    mov ecx, 5
    mov r8d, 3
    sub ecx, r8d
    sub edx, ecx
    sub edi, edx
    mov edx, 5
    mov ecx, 3
    add edx, ecx
    mov ecx, 10
    sub ecx, edx
    mov edx, 15
    mov r8d, 5
    add edx, r8d
    mov r8d, 5
    mov r9d, 2
    add r8d, r9d
    sub edx, r8d
    add ecx, edx
    add edi, ecx
    add esi, edi
    mov edi, 50
    mov edx, 60
    add edi, edx
    mov edx, 30
    mov ecx, 20
    sub edx, ecx
    mov ecx, 10
    mov r8d, 5
    sub ecx, r8d
    add edx, ecx
    sub edi, edx
    mov edx, 40
    mov ecx, 25
    sub edx, ecx
    mov ecx, 35
    mov r8d, 15
    sub ecx, r8d
    add edx, ecx
    add edi, edx
    mov edx, 20
    mov ecx, 10
    add edx, ecx
    mov ecx, 5
    mov r8d, 5
    add ecx, r8d
    sub edx, ecx
    mov ecx, 15
    mov r8d, 5
    sub ecx, r8d
    mov r8d, 10
    mov r9d, 5
    add r8d, r9d
    sub ecx, r8d
    add edx, ecx
    sub edi, edx
    mov edx, 25
    mov ecx, 35
    add edx, ecx
    mov ecx, 15
    mov r8d, 10
    sub ecx, r8d
    sub edx, ecx
    mov ecx, 20
    mov r8d, 10
    sub ecx, r8d
    mov r8d, 15
    mov r9d, 5
    sub r8d, r9d
    add ecx, r8d
    add edx, ecx
    mov ecx, 10
    mov r8d, 10
    add ecx, r8d
    mov r8d, 5
    mov r9d, 2
    sub r8d, r9d
    sub ecx, r8d
    mov r8d, 5
    mov r9d, 5
    add r8d, r9d
    mov r9d, 3
    mov r10d, 1
    add r9d, r10d
    sub r8d, r9d
    add ecx, r8d
    sub edx, ecx
    add edi, edx
    sub esi, edi
    mov edi, 100
    mov edx, 50
    sub edi, edx
    mov edx, 75
    mov ecx, 25
    sub edx, ecx
    add edi, edx
    mov edx, 60
    mov ecx, 40
    add edx, ecx
    mov ecx, 30
    mov r8d, 20
    sub ecx, r8d
    sub edx, ecx
    sub edi, edx
    mov edx, 45
    mov ecx, 35
    add edx, ecx
    mov ecx, 25
    mov r8d, 15
    add ecx, r8d
    sub edx, ecx
    mov ecx, 20
    mov r8d, 10
    sub ecx, r8d
    mov r8d, 10
    mov r9d, 5
    sub r8d, r9d
    sub ecx, r8d
    add edx, ecx
    add edi, edx
    mov edx, 15
    mov ecx, 15
    add edx, ecx
    mov ecx, 10
    mov r8d, 5
    add ecx, r8d
    sub edx, ecx
    mov ecx, 5
    mov r8d, 5
    add ecx, r8d
    mov r8d, 3
    mov r9d, 2
    add r8d, r9d
    sub ecx, r8d
    add edx, ecx
    mov ecx, 30
    mov r8d, 20
    sub ecx, r8d
    mov r8d, 25
    mov r9d, 15
    sub r8d, r9d
    add ecx, r8d
    mov r8d, 10
    mov r9d, 5
    add r8d, r9d
    mov r9d, 5
    mov r10d, 3
    sub r9d, r10d
    add r8d, r9d
    sub ecx, r8d
    sub edx, ecx
    sub edi, edx
    mov edx, 40
    mov ecx, 60
    add edx, ecx
    mov ecx, 30
    mov r8d, 20
    sub ecx, r8d
    mov r8d, 10
    mov r9d, 5
    sub r8d, r9d
    add ecx, r8d
    sub edx, ecx
    mov ecx, 25
    mov r8d, 15
    sub ecx, r8d
    mov r8d, 20
    mov r9d, 10
    sub r8d, r9d
    add ecx, r8d
    add edx, ecx
    mov ecx, 15
    mov r8d, 10
    add ecx, r8d
    mov r8d, 5
    mov r9d, 5
    add r8d, r9d
    sub ecx, r8d
    mov r8d, 10
    mov r9d, 5
    sub r8d, r9d
    mov r9d, 5
    mov r10d, 2
    add r9d, r10d
    sub r8d, r9d
    add ecx, r8d
    sub edx, ecx
    add edi, edx
    add esi, edi

    ret

