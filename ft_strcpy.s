global _ft_strcpy

_ft_strcpy:
	xor	rcx, rcx

while:
	mov al, [rsi + rcx]
	mov [rdi + rcx], al
	inc rcx
	cmp al, 0
	jne while

break:
	mov rax, rdi
	ret
