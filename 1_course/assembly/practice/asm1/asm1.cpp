#include <iostream>

int main() {
    int a, b, c, d, result, rest;
    std::cout << "Введите 4 числа для проведения над ними операций: \n";
    std::cin >> a >> b >> c >> d;
    __asm {
        mov eax, a
        mul eax
        mov ecx, eax
        mov eax, b
        mov ebx, 10
        mul ebx
        add ecx, eax
        mov eax, c
        mov ebx, 3
        mul ebx
        sub ecx, eax
        mov eax, d
        mov ebx, 40
        mul ebx
        mov ebx, eax
        mov eax, ecx
        div ebx
        mov result, eax
        mov rest, edx
    }
    std::cout << "Часное: " << result << ", остаток: " << rest << std::endl;
    return 0;
}