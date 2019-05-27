PUBLIC asm_letters_needed

.386

.model flat, c

.code

;--------------------------------------------------------
asm_letters_needed PROC
;
; Function that takes two strings, and figures out how
; many letters need to be removed from the first one
; so that it equals the second one. If no number of letters
; can do it, it will return -1.
; 
; 
; 
; Receives (on stack):
;	char * pointing at the first, longer string
;   char * pointing at the second, shorter string
; 
; Returns: 
;	EAX: the number of letters from the first string needed 
;	to be removed to equal the second string. Returns -1 if
;	the strings cannot be made equal
;--------------------------------------------------------
	push ebp

	mov ebp, esp

	push esi

	mov ebp, esp ;mov into the right registers
	mov eax, [ebp + 8]
	mov esi, [ebp + TYPE DWORD + 8]

	mov ecx, 0

redo:
	mov bl, byte ptr [eax]
	
	cmp bl, 0 ;at end of string?
	je donestring

	cmp bl, byte ptr [esi] ;are they equal
	je equal

	inc ecx ;if not, we need to increment the wrong coiunter, but not increment esi
	inc eax

	jmp redo

equal:
	inc eax ;if theyre equal, go to the next letter and redo
	inc esi

	jmp redo

donestring:
	mov bl, byte ptr [esi] ;is the second string at the end too?
	cmp bl, 0
	jne retnul

	mov eax, ecx ;return the number of letters needed

	pop esi
	pop ebp
	ret

retnul:
	mov eax, -1 ;return an error code

	pop esi
	pop ebp
	ret

asm_letters_needed ENDP

end