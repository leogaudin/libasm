global _ft_write
extern ___error ; TO CHANGE TO errno_location WHEN SUBMITTING ON LINUX

_ft_write:
    mov rax, 0x2000004 ; sys_write syscall number TO CHANGE WHEN SUBMITTING ON LINUX
	; mov rdi, rdi ; file descriptor
    ; mov rsi, rsi ; buffer
    ; mov rdx, rdx ; count
	syscall
	jc error
	ret

error:
	mov r8, rax ; syscall returned errno in rax, so we save it before overwriting it with ___error
	call ___error ; TO CHANGE TO errno_location WHEN SUBMITTING ON LINUX
	mov [rax], r8 ; rax has the address of errno, so we put the previously saved error code in [rax]
	mov rax, -1 ; write returns -1 in any error case
	ret
