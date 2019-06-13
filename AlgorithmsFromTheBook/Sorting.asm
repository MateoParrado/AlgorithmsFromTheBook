PUBLIC asm_sort_in_place
PUBLIC asm_sort_in_place_rev

.386

.model flat, c

.code

;--------------------------------------------------------
asm_sort_in_place PROC
;
; Sort an array of unsigned ints in place
;
; Receives:
;	A DWORD * pushed onto the stack, pointing to an array to be sorted
;   A DWORD of how much to sort
; 
; Returns: 
;	nothing
;--------------------------------------------------------
	push ebp

	mov ebp, esp

	mov ecx, [ebp+12] ;ecx holds count
	
	cmp ecx, 1
	jle return

outerloop:
	mov eax, [ebp+8] ;eax holds ptr

	push ecx

	mov ecx, [ebp+12] ;ecx holds count
	dec ecx

innerloop:
	mov ebx, [eax]
	mov esi, [eax + 4]
	cmp ebx, [eax+4] ; is the next integer greater than it
	jl continueLoop

	xchg ebx, [eax+4]
	mov [eax], ebx

continueLoop:
	add eax, 4
	loop innerloop

	pop ecx
	loop outerloop

return:
	pop ebp	

	ret
asm_sort_in_place ENDP

;--------------------------------------------------------
asm_sort_in_place_rev PROC
;
; Sort an array of unsigned ints in place
;
; Receives:
;	A DWORD * pushed onto the stack, pointing to an array to be sorted
;   A DWORD of how much to sort
; 
; Returns: 
;	nothing
;--------------------------------------------------------
	push ebp

	mov ebp, esp

	mov ecx, [ebp+12] ;ecx holds count
	
	cmp ecx, 1
	jle return

outerloop:
	mov eax, [ebp+8] ;eax holds ptr

	push ecx

	mov ecx, [ebp+12] ;ecx holds count
	dec ecx

innerloop:
	mov ebx, [eax]
	mov esi, [eax + 4]
	cmp ebx, [eax+4] ; is the next integer greater than it
	jg continueLoop

	xchg ebx, [eax+4]
	mov [eax], ebx

continueLoop:
	add eax, 4
	loop innerloop

	pop ecx
	loop outerloop

return:
	pop ebp	

	ret
asm_sort_in_place_rev ENDP

end