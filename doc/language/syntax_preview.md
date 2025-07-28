```
@entrypoint: main;


procdr sumtwo: int16 number -> int16
start
    return number + 2;
end


procdr main -> int16
start
    decl number1: int16 = 10;
    decl number2: int16 = 25;

    number2 = number2 + number1 * 5 - 7;

    decl result: int16 = sumtwo(1234);

    return 0;
end
```

should result:

```
bits 64


section .text

global _start
_start:
    mov rbp, rsp ; initialize stack frame

    call main
    mov di, ax

    ; sycall exit
    mov rax, 60 ; syscall code
    syscall



sumtwo:
    push rbp     ; new stack frame
    mov rbp, rsp ; start of new stack frame
    add rbp, 16  ; pointing directly to the arguments, ignore the return address and the pushed "rbp" (both 8 bytes)



    mov di, [rbp + 0] ; load argument "number"
    add di, 2



    ; close stack frame
    sub rbp, 16 ; restore original pointing address
    mov rsp, rbp
    pop rbp

    mov ax, di ; return value stored in register ax
    ret


main:
    push rbp     ; new stack frame
    mov rbp, rsp ; start of new stack frame
    add rbp, 16



    sub rsp, 2             ; allocate 2 bytes
    mov word [rsp + 0], 10 ; variable number1

    sub rsp, 2
    mov word [rsp + 0], 25 ; variable number2
    
    ; number2 = number2 + number1 * 5 - 7
    

    ; number1 * 5
    mov ax, word [rsp + 2]
    mov di, 5
    mul di

    mov si, word [rsp + 0] ; load number2

    add si, ax ; number2 + ...
    sub si, 7  ; number2 - 7

    mov word [rsp + 0], di ; number2 = ...


    sub rsp, 2
    mov word [rsp + 0], 1234 ; initializing parameter "number"

    call sumtwo ; return value in ax

    sub rsp, 2
    mov word [rsp + 0], ax ; variable "result"


    ; close stack frame
    sub rbp, 16
    mov rsp, rbp
    pop rbp

    mov ax, 0 ; return value in ax
    ret
```