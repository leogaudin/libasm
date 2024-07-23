global _ft_strlen

_ft_strlen:
	xor	rcx, rcx
	xor	rax, rax

while:
	cmp BYTE[rdi + rcx], 0
	je break
	inc rcx
	jmp while

break:
	mov rax, rcx
	ret
