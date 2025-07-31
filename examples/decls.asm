bits 64


section .data



section .text

global _start
_start:
	mov rbp, rsp

	mov rax, 60
	mov rdi, 0
	syscall


