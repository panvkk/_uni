.386
.model flat, c
PUBLIC convertToHex

.code

convertToHex PROC
    push ebp
    mov  ebp, esp
    push ebx
    push esi
    push edi

    mov eax, DWORD PTR [ebp+8]    
    mov edi, DWORD PTR [ebp+12]      

    test edi, edi
    jz done

    cmp eax, 0
    jne ConversionLoop
    mov byte ptr [edi], '0'
    mov byte ptr [edi+1], 0
    jmp done

ConversionLoop:
    xor ecx, ecx            

LoopStart:
    mov ebx, eax
    and ebx, 0Fh            
    cmp ebx, 9
    jbe DigitConvert        
    add ebx, 'A' - 10       
    jmp PushDigit
DigitConvert:
    add ebx, '0'           
PushDigit:
    push ebx                 
    inc ecx              
    shr eax, 4            
    test eax, eax
    jnz LoopStart          

PopLoop:
    cmp ecx, 0
    je AddTerminator      
    pop ebx                
    mov [edi], bl       
    inc edi
    dec ecx
    jmp PopLoop

AddTerminator:
    mov byte ptr [edi], 0  

done:
    pop edi
    pop esi
    pop ebx
    mov esp, ebp
    pop ebp
    ret
convertToHex ENDP
END