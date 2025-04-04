.586
.model flat, C
public getStatistics
public getParallax

.data
separators db ' ', ',', ';', '-', '?', '!', '#', '"', 9, 13, 10, 0
buffer dword 3 dup(?)

.code

getStatistics proc
	push ebp
	mov ebp, esp
	push ebx
	push esi		

	lea eax, buffer

	mov ecx, DWORD ptr [ebp + 8]
	mov esi, DWORD ptr [ebp + 12]

	xor edx, edx
	mov dword ptr [eax + 0], 0
	mov dword ptr [eax + 4], 0
	mov dword ptr [eax + 8], 0

	dec ecx
program_loop:
	mov bl, byte ptr [esi + ecx]

	push edx
	mov edx, dword ptr [eax]
	inc edx
	mov dword ptr [eax], edx
	pop edx

	cmp bl, 184
	je is_russian

	cmp bl, 192
	jl start_separators_check
	cmp bl, 255
	jg start_separators_check
	jmp is_russian

is_russian:
	push edx
	mov edx, dword ptr [eax + 8]
	inc edx
	mov dword ptr [eax + 8], edx
	pop edx	

start_separators_check:
	push ecx
	push edx
	mov ecx, 11
separators_loop:
	mov dl, byte ptr [separators + ecx]
	cmp bl, dl
	je is_separator
	cmp ecx, 0
	je separators_loop_end
	dec ecx
	jmp separators_loop
separators_loop_end:
	pop edx
	pop ecx
	jmp retry_loop
is_separator:
	pop edx
	pop ecx
	inc edx

retry_loop:
	cmp ecx, 0
	je program_end
	dec ecx
	jmp program_loop
program_end:
	mov dword ptr [eax + 4], edx
	pop esi			
	pop ebx
	mov esp, ebp
	pop ebp 
	ret
getStatistics endp

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

getParallax proc
    push ebp             
    mov ebp, esp
    sub esp, 40         

    push ebx
    push esi
    push edi

    mov edi, dword ptr [ebp+8]

   
    lea esi, [edi]       
    lea edx, [ebp-40]    
    mov ecx, 10 
copy_loop:
    mov eax, dword ptr [esi] 
    mov dword ptr [edx], eax 
    add esi, 4       
    add edx, 4
    loop copy_loop

    mov ebx, 0         
parallax_loop:
    mov eax, ebx
    add eax, 9  
    cmp eax, 10
    jl no_wrap_parallax
    sub eax, 10
no_wrap_parallax:
    mov edx, dword ptr [ebp-40 + eax*4]
    mov esi, dword ptr [ebp+8]
    mov dword ptr [esi + ebx*4], edx

    inc ebx
    cmp ebx, 10
    jl parallax_loop

    pop edi
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret
getParallax endp

END