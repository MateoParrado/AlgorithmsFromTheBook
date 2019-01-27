PUBLIC asm_longest_palindromic_substring

.386

.model flat, c

.data

firstArrayPtr DWORD ?
strSize DWORD ?

.code

;--------------------------------------------------------
asm_longest_palindromic_substring PROC
;
; Function that finds the length of the longest palindrome
; in a string. The function will only work in 32 bit mode, 
; and will break for any lengths of palindrome greater than
; 255. It finds it in o(n^2) time.
; 
; Receives:
;	a char * on the stack
; 
; Returns: 
;	AL: the length of the longest palindrome in the string
;--------------------------------------------------------
	push ebp

	mov ebp, esp

	;get char from stack
	mov eax, [ebp + 8]

	;the distance we are from the center of our palindrome
	mov dl, 0

	;count length of string
	mov ecx, -1

	;point at right location
	sub ebp, 4

preprocess:
	;count the letters
	add ecx, 2

	;add intermediate zeros
	mov [ebp], dl
	dec ebp

	;add the charachter
	mov bl, byte ptr [eax]
	mov [ebp], bl

	;check next letters
	inc eax
	dec ebp

	;if its not zero, keep going
	test bl, bl
	jnz preprocess

	;change eax to point to our newly created array
	mov edx, ebp
	mov eax, edx
	inc eax
	inc eax

	;store string size
	mov strSize, ecx

	;for bounds checking
	mov edi, eax
	mov firstArrayPtr, edi
	add edi, ecx

	jmp skipTheFirstTime

	;loop through every point
calculationLoop:
	dec ebp
	inc eax

skipTheFirstTime:

	;the range being checked
	mov esi, 1

	;the number to be written
	mov byte ptr [ebp], 0
	mov ecx, ecx
expandLoop:
	;check one above
	mov edx, eax
	add edx, esi

	;bounds checking
	cmp edx, edi
	jg done
	mov bl, byte ptr[edx]

	;check one below
	sub edx, esi
	sub edx, esi

	;lower bounds checking
	cmp edx, [firstArrayPtr]
	jl calculationLoop
	mov bh, byte ptr[edx]
	
	;if they arent equal, keep looping
	cmp bh, bl
	loopnz calculationLoop

	;if you've reached this point, add one to size, and grow
	inc ecx;janky fix to loopnz subtraction
	inc esi
	inc byte ptr [ebp]

	jmp expandLoop

done:
	;point esi to the array
	mov esi, ebp
	inc esi

	;start looping
	mov ecx, strSize

	;mov al, 0
	xor al, al

keepLooping:

	mov dl, byte ptr [esi]

	;is the value now pointed to greater than the value that was previously pointed to
	cmp dl, al
	jng doLoop

	;if yes, store it
	mov al, dl

doLoop:
	;point at next value and keep looping
	inc esi
	loop keepLooping

	pop ebp

	ret

asm_longest_palindromic_substring ENDP

end