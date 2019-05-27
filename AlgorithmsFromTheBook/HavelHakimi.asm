PUBLIC asm_havel_hakimi

.386

.model flat, c

.code

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

	mov eax, [ebp+8] ;eax holds the ptr
	mov ecx, [ebp+12] ;ecx holds the count

	;rewrite the array in a new location with the zeros removed
	push ecx

	mov ebx, -4
	mov edx, -4

keepRewriting:
	add ebx, 4

	mov esi, dword ptr [eax + ebx]

	cmp esi, 0
	loope keepRewriting

	add edx, 4

;	mov byte ptr [eax - edx], esi

	loop keepRewriting

	pop ecx

	;call the sorting proc
	push ecx
	push eax

;	call asm_sort_in_place

	pop eax
	pop ecx

	pop ebp

	ret
asm_havel_hakimi ENDP

end