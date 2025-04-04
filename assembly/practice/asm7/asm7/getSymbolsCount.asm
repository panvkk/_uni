.586
.model flat, C
public getWordsCount

.data
separators db ":;!?.,- \|/()[]{}",9,10,0

.code

getWordsCount proc
	push ebp		; Пролог
	mov ebp, esp
	push ebx
	push esi		
	mov ecx, DWORD ptr [ebp + 8]
	mov esi, DWORD ptr [ebp + 12] ; закидываю массив
	xor eax, eax
	xor edx, edx

first_loop:

	mov bl, BYTE ptr [esi + ecx]
	cmp bl, '0'
	jl check_alpha
	cmp bl, '9'
	jle is_alnum
check_alpha:
	cmp bl, 'A'
	jl check_lower
	cmp bl, 'Z'
	jle is_alnum
check_lower:
	cmp bl, 'a'
	jl is_not_alnum
	cmp bl, 'z'
	jle is_alnum

is_alnum:
	mov edx, 1
	jmp retry_loop
is_not_alnum:
	cmp edx, 1
	je is_was_word
	jmp retry_loop
is_was_word:
	inc eax
	mov edx, 0
	jmp retry_loop
retry_loop:
	cmp ecx, 0
	je end_first_loop
	dec ecx
	jmp first_loop
end_first_loop:
	cmp edx, 1
	jne program_end
	inc eax
program_end:
	pop esi			; Эпилог 
	pop ebx
	mov esp, ebp
	pop ebp 
	ret
getWordsCount endp
END