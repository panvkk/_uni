#include <iostream>
#include <cstring>

extern "C" int __cdecl convertHexToFixed(int length, char* hexArray);

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    char hexDigits[20] = { 0 }; 
    std::cout << "Введите целое число в шестнадцатеричной системе счисления: ";
    std::cin >> hexDigits;

    int len = (int)std::strlen(hexDigits);

    int result = convertHexToFixed(len, hexDigits);

    std::cout << "Преобразованное число: " << result << std::endl;
    system("pause");
    return 0;
}
