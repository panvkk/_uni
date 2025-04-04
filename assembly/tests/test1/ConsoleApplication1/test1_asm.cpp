#include <iostream>
#include <windows.h>
#include <fstream>

extern "C" int* __cdecl getStatistics(int length, char* symbols);

extern "C" void __cdecl getParallax(int* nums);

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleCP(1251);

    char line[101];
    int numbers[10] { 0 };
    int* statistics;
    int len;
    char curr;
    int count;

    // Дедушка! Файл Input.txt должен быть кодировки Windows-1251, только так всё будет работать!
    std::ifstream file("Input.txt");

    if (file.is_open()) {
        std::cout << "Файл успешно открыт для чтения." << std::endl;
    }
    else {
        std::cout << "Не удалось открыть файл для чтения. \nПроверьте, находится ли он в одной директории с исполнимым модулем.." << std::endl;
        system("pause");
        return 1;
    }

    for (count = 0; file.get(curr) && curr != '\x1A'; count++) {
        if (count == 100) {
            std::cout << "Слишком много символов! Подсчёт будет вестись только для первых 100.\n";
            break;
        }
        line[count] = curr;
    }

    line[count] = '\0';
    len = (int)std::strlen(line);
    
    statistics = getStatistics(len, line);

    std::cout << "\nКоличество символов в строке:\ " << statistics[0]
        << "\nКоличество разделителей: " << statistics[1]
        << "\nКоличество русских букв: " << statistics[2] << std::endl;

    std::cout << "\nВведите 10 чисел через пробел:\n";
    for (int i = 0; i < 10; i++)
        std::cin >> numbers[i];

    getParallax(numbers);

    std::cout << "10 чисел после цикличного смещения влево(9 раз): \n";
    for (int i = 0; i < 10; i++)
        std::cout << numbers[i] << " ";
    std::cout << "\n";
    system("pause");
    return 0;
}