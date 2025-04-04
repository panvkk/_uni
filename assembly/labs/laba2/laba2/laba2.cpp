#include <iostream>

extern "C" void __cdecl convertToHex(unsigned int number, char* hexArray);

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    unsigned int number;
    std::cout << "Введите целое число: ";
    std::cin >> number;

    char hexDigits[9] = { 0 }; // 8 символов для представления 32-битного числа + завершающий ноль
    convertToHex(number, hexDigits);

    std::cout << "Шестнадцатеричное представление: " << hexDigits << std::endl;
    system("pause");
    return 0;
}
