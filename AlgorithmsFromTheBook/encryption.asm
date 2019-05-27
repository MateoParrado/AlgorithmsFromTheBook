PUBLIC asm_rot_encrypt

.386

.model flat, c

.data
key BYTE -2, 4, 1, 0, 3, 5, 2, 4, 4, 6

.code

;--------------------------------------------------------
asm_rot_encrypt PROC
;
; Encrypts a null terminated string of any length
; by rotating every charachter in it according to the
; key above.
; 
; Receives:
;	pointer to the string on the stack
; 
; Returns: 
;	nothing
;--------------------------------------------------------
	push ebp
	
	mov ebp, esp

	mov eax, [ebp + 8]

	push esi
	push ebx

	mov esi, offset key

keepShifting:
	mov ebx, [esi]
	cmp ebx, 0

	jb negative;if negative negate it
	je done;if equal do nothing
	
	mov cl, byte ptr[esi]
	ror byte ptr [eax], cl;rotate right
	jmp done

negative:
	mov cl, byte ptr [esi];negate it and rotate left
	neg cl
	rol byte ptr [eax],  cl

done:
	inc eax;point at the next thing
	inc esi
	
	cmp cl, 6;6 is the last value, so if we just used it (aka its in cl) go to zero
	jne keepgoing

	mov esi, offset key

keepgoing:

	mov ebx, [eax]
	cmp ebx, 0 ;if the null char is reached, finish
	jne keepShifting

	pop ebx
	pop esi

	pop ebp
	ret
asm_rot_encrypt ENDP

end