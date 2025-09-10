.586
.model flat, C

.data
separators db ":;!?.,- \|/()[]{}",9,10,0

.code
PUBLIC collectStat

collectStat proc C symbolsSourceLink:dword, symbolsAmountLink:dword, wordCountLink:dword
	mov dl, 0 ; isInWord = false
	mov eax, 0
	mov ebx, 0 ; wordCounter = 0
	mov esi, symbolsSourceLink
	mov edi, symbolsAmountLink
	while1:
		mov al, [esi]
		cmp al, 0
		jz endWhile1

		mov cx, word ptr [edi + eax*2]
		add cx, 1

		mov [edi + eax*2], cx

		
		push edi
		mov edi, 0
		mov dh, 0
		while2:
			cmp separators[edi], byte ptr 0
			je endWhile2
				
			cmp dh, 1
			je endWhile2


			mov al, separators[edi]
			iff4:
				cmp al, [esi]
				jne else4

				mov dh, 1
				jmp endIf4

				else4:
					mov dh, 0
			endIf4:

			add edi, 1

			jmp while2
		endWhile2:
		pop edi
		iff1:
			cmp dh, 1
			jne else1

			iff2:
				cmp dl, 1
				jne endIf2
				add ebx, 1
			endIf2:
			mov dl, 0
			jmp endIf1

			else1:
				mov dl, 1
		endIf1:

		add esi, 1
		jmp while1
	endWhile1:

	if3:
		cmp dl, 1
		jne endIf3
		add ebx, 1
	endIf3:

	mov esi, wordCountLink
	mov [esi], ebx

	ret
collectStat endp
end