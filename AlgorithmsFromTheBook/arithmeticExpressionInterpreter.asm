PUBLIC asm_arithmetic_expression_calculator

.386

.model flat, c

.code

;helper proc to the one below, defines the operators as special charachters -1 : -4 to be used by the tokenizer and parser
tokenize PROC
	.IF byte ptr [eax] == '*'
		mov ebx, -1
	.ELSEIF byte ptr [eax] == '/'
		mov ebx, -2
	.ELSEIF byte ptr [eax] == '-'
		mov ebx, -3
	.ELSEIF byte ptr [eax] == '+'
		mov ebx, -4
	.ENDIF
	ret
tokenize ENDP

;--------------------------------------------------------
asm_arithmetic_expression_calculator PROC
;
; Function that takes in a char * pointing to a string 
; representing an unparenthesized arithmetic expression using
; only the operators + - * or /. It then returns the calculated 
; value of that expression. Whitespace may be used. Does not support
; negative numbers. Executes order of operations from left to right, 
; meaning that 4 * 2 / 4 returns 0.
;
; Receives:
;	a char * pointing to the expression on the stack
; 
; Returns: 
;	EAX: an integer that is the result of calculating the string
;--------------------------------------------------------
	push ebp

	mov ebp, esp

	;eax now holds the pointer to the char
	mov eax, [ebp + 8]
	dec eax

	;point ebp to the right location
	sub ebp, 4
	
	;esi = 0, will hold the temp value
	xor esi, esi
	xor ecx, ecx
	xor edx, edx

nextChar:
	inc eax
	cmp byte ptr [eax], 0
	je doneTokenizing
	inc ecx

	cmp byte ptr [eax], ' '
	je nextChar

	;dl holds the char
	mov dl, byte ptr [eax]
	sub dl, '0'

	;go to operator if it isnt a number
	cmp dl, 9
	ja operator
	

	;multiply by ten and add the new char
	imul esi, esi, 10
	add esi, edx
	jmp nextChar

operator:
	sub ebp, 4
	mov dword ptr [ebp], esi

	call tokenize
	sub ebp, 4
	mov dword ptr [ebp], ebx

	xor esi, esi

	jmp nextChar

doneTokenizing:
	;add the last value on there
	sub ebp, 4
	mov dword ptr[ebp], esi

	;eax is now our return value
	mov eax, dword ptr [ebp]

	dec ecx

	xor edx, edx

keepParsing:
	add ebp, 4

	mov esi, dword ptr [ebp]

	.IF dword ptr [ebp] == -1;multiply
		add ebp, 4
		mul dword ptr [ebp]
		dec ecx
	.ELSEIF dword ptr [ebp] == -2;divide
		add ebp, 4

		mov ebx, eax
		mov eax, dword ptr [ebp]
		div ebx

		dec ecx
	.ELSEIF dword ptr [ebp] == -3;subtract
		dec ecx
		add ebp, 4

		;tempval to be added
		mov ebx, [ebp]

	recheckMultiply:

		add ebp, 4

		.IF dword ptr [ebp] == -1;multiply
			add ebp, 4
			imul ebx, dword ptr [ebp]

			sub ecx, 2

			cmp ecx, 0
			jg recheckMultiply
		.ELSEIF dword ptr [ebp] == -2;divide
			add ebp, 4

			push eax

			mov eax, dword ptr [ebp]

			div ebx

			mov ebx, eax

			pop eax

			sub ecx, 2

			cmp ecx, 0
			jg recheckMultiply
		.ENDIF	
		
		sub ebx, eax
		mov eax, ebx

		dec ecx
		sub ebp, 4
	.ELSEIF dword ptr[ebp] == -4;add
		add ebp, 4

		;tempval to be added
		mov ebx, [ebp]

	checkMultiply:

		add ebp, 4

		.IF dword ptr [ebp] == -1;multiply
			add ebp, 4
			imul ebx, dword ptr [ebp]

			sub ecx, 2

			cmp ecx, 0
			jg checkMultiply
		.ELSEIF dword ptr [ebp] == -2;divide
			add ebp, 4

			push eax

			mov eax, dword ptr [ebp]

			div ebx

			mov ebx, eax

			pop eax

			sub ecx, 2

			cmp ecx, 0
			jg checkMultiply
		.ENDIF	
		
		dec ecx
		add eax, ebx

		sub ebp, 4
	.ENDIF

	dec ecx

	cmp ecx, 0
	jg keepParsing

	pop ebp

	ret
asm_arithmetic_expression_calculator ENDP

end