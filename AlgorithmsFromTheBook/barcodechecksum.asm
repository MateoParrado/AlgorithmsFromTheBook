PUBLIC asm_get_barcode_checksum

.386

.model flat, c

.data
sum DWORD ?

.code

asm_get_barcode_checksum PROC
	push ebp

	mov sum, 0 ;reset the sum counter to remove previous loop interference
	
	mov ebp, esp

	mov ebx, 0 ;bl will hold data, so ebx must be clear
	
	mov ecx, eax ;eax holds ptr, but now ecx does

	mov eax, 0 ;eax is counter
L1:
	inc eax

	mov bl, byte ptr [ecx] ;mov the char from ecx into bl

	push eax ;multiply it by three and add it to sum
	mov eax, 3
	mul ebx
	add sum, eax
	pop eax
	
	inc ecx ;point at the next char

	cmp eax, 11 ;if this has been done 11 times, leave the loop
	je outofloop

	inc eax ;add the next char to sum
	mov bl, byte ptr[ecx]
	add sum, ebx

	inc ecx ;point at the next char

	jmp L1

outofloop:
	mov edx, 0 ;divide the sum by 10
	mov eax, sum
	mov ebx, 10
	div ebx

	cmp edx, 0 ;if there is no remainder, go to zero case
	je zero

	mov ebx, 10 ;subtract the remainder from ten and jump to not zero case
	sub ebx, edx
	jmp nzero

zero:
	mov eax, 0 ;return zero
	jmp en;d

nzero:
	mov eax, ebx ;mov ret val to eax
en:
	pop ebp
	ret
asm_get_barcode_checksum ENDP

end