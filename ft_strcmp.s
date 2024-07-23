global _ft_strcmp

_ft_strcmp:
	xor rcx, rcx
	xor rax, rax
	xor r8, r8

while:
	movzx rax, BYTE[rdi + rcx]
	movzx r8, BYTE[rsi + rcx]

	cmp rax, 0
	cmp r8, 0
	jz break
	cmp rax, r8
	jne break

	inc rcx
	jmp while

break:
	sub rax, r8
	ret
