global _ft_strdup
extern _malloc
extern _ft_strcpy
extern _ft_strlen

_ft_strdup:
; rdi contains the *s to duplicate
	call	_ft_strlen
; rax now contains strlen(*s)
; rdi still contains the *s to duplicate
	push	rdi	; We save *s in the stack
	inc		rax
	mov		rdi, rax ; We move strlen(*s) in rdi so that malloc can read it
	call	_malloc
; rax now contains a pointer to the newly-allocated space of length strlen(*s)
	pop		rsi ; Put back the *s to duplicate, in rsi (second argument)
	mov		rdi, rax ; Put the newly-allocated space in rdi (first argument)
	call	_ft_strcpy
; rax still contains a pointer to the now duplicate string
	ret
