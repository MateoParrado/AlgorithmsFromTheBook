PUBLIC asm_even_odd

.386

.model flat, c

.code

asm_even_odd PROC
	mov ecx, 0ffffffffh;
L1:
	mov eax, ecx
L2:
	cmp eax, 0
	jz quit
	cmp eax, 1
	je quit
	mul eax
	jmp L2

quit:
	
	mov ebx, ecx
	and ebx, 1

	cmp ebx, eax
	je quit2

	mov eax, 0
	div ebx

quit2:
	loop L1

	ret
asm_even_odd ENDP

end