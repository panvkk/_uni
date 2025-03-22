#include <iostream>

int main() {
    std::cout << "Ъелло\n";
    int a, b, c, d;
    __asm (
        mov eax, a
        mul eax
        mov ecx, eax
        mov eax, b
        mul 10
        add ecx, eax
        mov eax, c
        mul 3
        add ecx, eax
        mov eax, d
        mul 40
        mov ebx, eax
        mov eax, ecx
        div ebx
    )
    return 0;
}