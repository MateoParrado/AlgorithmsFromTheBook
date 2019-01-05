PUBLIC asm_xorshift

.386

.model flat, c

.code

;--------------------------------------------------------
asm_xorshift PROC
;
; Function that generates a pseudorandom number
; using a the xorshift method, a type of linear
; feedback shift register. Note that the numbers
; do not appear fully random, and are not usable 
; at full size. They should be modded down to a 
; smaller number.
; 
; Receives:
;	a seed dword on the stack
; 
; Returns: 
;	EAX: a pseudorandom number
;--------------------------------------------------------
	push ebp

	mov ebp, esp
	mov eax, [ebp + 8];take the value off the stack and into eax
	
	mov ebx, eax ;xor eax with itself shifted
	shl ebx, 13
	xor eax, ebx

	mov ebx, eax;do it again
	sar ebx, 17
	xor eax, ebx

	mov ebx, eax;and again
	shl ebx, 5
	xor eax, ebx;eax is returned

	pop ebp

	ret
asm_xorshift ENDP

end