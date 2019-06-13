PUBLIC asm_havel_hakimi

.386

.model flat, c

.code

asm_sort_in_place proto
asm_sort_in_place_rev proto

;--------------------------------------------------------
asm_havel_hakimi PROC
;
; A bunch of dudes are in a room, being interrogated. The interrogators
; ask each one of them how many of the other people have they met, and they
; each respond a number. Can they be telling the truth. 
;
; ALGORITHM:
;	1- remove all zeros
;	2- sort it in descending order
;	3- check if the first element is greater than the total length, if it is return lie
;	4- if not, subtract 1 from each of the next first element elements, push the ptr forwards and go to step 3
;
; Receives:
;	A DWORD * pushed onto the stack, pointing to an array of the number of people each person claims to have met
;   A DWORD of how many people there are, pushed onto the stack
; 
; Returns: 
;	AL: a boolean saying wether or not its possible
;--------------------------------------------------------
	push ebp

	mov ebp, esp

	mov eax, [ebp]

	mov eax, [ebp+8] ;eax holds the ptr
	mov ecx, [ebp+12] ;ecx holds the count

	push esi
	push edi

	;rewrite the array in a new location with the zeros removed
	mov ebx, -4
	mov edx, ebp

	mov edi, ecx
	jmp keepRewriting

subtractFromEdi:
	dec edi

keepRewriting:
	dec ecx

	add ebx, 4

	mov esi, dword ptr [eax + ebx]

	cmp esi, 0
	je subtractFromEdi

	sub edx, 4

	mov dword ptr [edx], esi

	test ecx, ecx
	jnz keepRewriting

	comment&

	shl edi, 2
	sub esp, edi
	shr edi, 2

	push edi
	push edx

	;call the sorting proc
	;call asm_sort_in_place_rev

	pop edx
	pop edi

	&
	mov esp, ebp
	add esp, 4

nextPerson:
	mov eax, [edx]
	cmp dword ptr [edx], edi
	jg retFalse ;if the value in the position is greater than then length, return false

	;otherwise, subtract from all the other ones and keep going
	mov ecx, dword ptr [edx]
	add edx, 4
	mov eax, edx

keepSubbing:
	dec dword ptr [eax]
	add eax, 4
	loop keepSubbing

	;subtract from edi and keep going
	dec edi
	jnz nextPerson

	pop edi
	pop esi

	pop ebp

	mov al, 1
	ret

retFalse:

	pop esi
	pop edi

	pop ebp

	xor al, al
	ret
asm_havel_hakimi ENDP

end