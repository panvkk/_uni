#include <iostream>
#include <windows.h>
#include <fstream>
#include <string>
#include <regex>
#include <iterator>

int main()
{
    setlocale(LC_ALL, "ru_RU.UTF-8");
    SetConsoleCP(1251);

    int len;
    char curr;

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
    std::string content((std::istreambuf_iterator<char>(file)),
                         std::istreambuf_iterator<char>());

    int symbols_count = content.size(), separators_count = 0, russians_count = 0;

    std::regex russian_regex(R"(([\xC0-\xFF\xB8]))");
    for (std::sregex_iterator it(content.begin(), content.end(), russian_regex);
         it != std::sregex_iterator(); ++it) {
        russians_count++;
    }

    std::regex sep_regex(R"([^A-Za-z0-9\xC0-\xFF\xB8])");
    for (std::sregex_iterator it(content.begin(), content.end(), sep_regex);
         it != std::sregex_iterator(); ++it) {
        separators_count++;
    }

    std::cout << "\nКоличество символов в строке: " << symbols_count
        << "\nКоличество разделителей: " << separators_count
        << "\nКоличество русских букв: " << russians_count << std::endl;

    return 0;
}
