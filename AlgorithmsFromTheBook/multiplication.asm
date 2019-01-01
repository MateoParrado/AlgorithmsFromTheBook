PUBLIC asm_multiply

.386

.model flat, c

.code

asm_multiply PROC
	;get the two parameters, put one in ebc and the other in edx
	push ebp
	mov ebp, esp
	mov ebx, [ebp + 8]
	mov edx, [ebp + TYPE DWORD + 8]

	mov eax, 0
	mov cl, 31
Redo:
	;if ecx is zero, youre done
	cmp edx, 0

	jne notZero

	pop ebp

	ret

notZero:
	test edx, 80000000h
	jz shift

	mov esi, [ebp + 8]
	shl esi, cl

	add eax, esi
shift:
	shl edx, 1
	dec cl

	jmp redo

asm_multiply ENDP

end