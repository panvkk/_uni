.386
.model flat, c
PUBLIC convertHexToFixed
.code

convertHexToFixed PROC
	push ebp
	mov ebp, esp
	push ebx
	push esi
	mov ecx, DWORD PTR [ebp+8]
	mov esi, DWORD PTR [ebp+12]
	xor eax, eax
convert_loop:
	cmp ecx, 0
	je convert_done
	mov bl, byte ptr [esi]
	cmp bl, '0'
	jl check_alpha
	cmp bl, '9'
	jle numeric_digit
check_alpha:
	cmp bl, 'A'
	jl check_lower
	cmp bl, 'F'
	jle alpha_upper
check_lower:
	cmp bl, 'a'
	jl invalid_digit
	cmp bl, 'f'
	jle alpha_lower
	invalid_digit:
	xor bl, bl
	jmp process_digit
numeric_digit:
	sub bl, '0'
	jmp process_digit
	alpha_upper:
	sub bl, 'A'
	add bl, 10
	jmp process_digit
alpha_lower:
	sub bl, 'a'
	add bl, 10
process_digit:
	movzx ebx, bl
	shl eax, 4
	add eax, ebx
	inc esi
	dec ecx
	jmp convert_loop
convert_done:
	pop esi
	pop ebx
	mov esp, ebp
	pop ebp
	ret
convertHexToFixed ENDP
END
