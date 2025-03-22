.MODEL FLAT, C
.STACK 4096

.DATA
desyat dd 10

.CODE
PUBLIC get_symbols

get_symbols PROC

    mov eax, [esp+4]
    mov esi, [esp+8]

    mov ecx, 0
    cmp eax, 0
    jne conv_loop
    push '0'
    inc ecx
    jmp pop_digits

conv_loop:
    cmp eax, 0
    je pop_digits
    mov edx, 0
    div desyat
    add edx, 48
    push edx
    inc ecx
    jmp conv_loop
pop_digits:
    cmp ecx, 0
    je finish
pop_loop:
    pop edx
    mov byte ptr [esi], dl
    inc esi
    dec ecx
    cmp ecx, 0
    jne pop_loop
finish:
    mov byte ptr [esi], 0
    ret
get_symbols ENDP

END