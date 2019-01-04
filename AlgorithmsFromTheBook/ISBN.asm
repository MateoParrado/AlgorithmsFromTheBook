PUBLIC asm_check_ISBN

.386

.model flat, c

.data
sum DWORD ?

.code

asm_check_ISBN PROC
	push ebp

	mov sum, 0 ;reset the sum counter to remove previous loop interference
	
	mov ebp, esp

	mov ebx, 0 ;bl will hold data, so ebx must be clear
	
	mov ebx, eax;mov ptr to ebx

	mov ecx, 10 ;loop ten times for the ten numbers

	mov eax, 0;so the lower part of it can be used
L1:
	mov al, byte ptr [ebx] ;move the char to al
	
	mul ecx ;multiply the char by the descending number

	add sum, eax

	inc ebx ;point to next char

	loop L1

	mov eax, sum ;find x mod 11
	mov ebx, 11
	div ebx

	mov eax, edx ;to return mov to eax

	cmp eax, 0 ;we need to switch true and false (0 is valid, anything else is invalid)

	jne nZero ;if not zero, go to nZero

	mov eax, 1;if it is zero, make it true
	jmp quit
	
nZero:
	mov eax, 0 ;if its not zero, make it false

quit:
	pop ebp
	ret
asm_check_ISBN ENDP

end