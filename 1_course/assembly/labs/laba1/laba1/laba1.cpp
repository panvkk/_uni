#include <iostream>
extern "C" void get_symbols(unsigned int number, char* array); // Прототип функции из ассемблера

int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");
    unsigned int num;
    char symbols[10];
    std::cout << "Здравстуйте, дедушка.\nВведите целое число, для получения его в массив в виде символов:\n";
    std::cin >> num;
    get_symbols(num, symbols);

    for (int i = 0; symbols[i] != '\0'; i++)
        std::cout << symbols[i] << " ";
    system("pause");
    return 0;
}
