#include "Date.h"
#include <iostream>
#include <fstream>
#include <string>

// Функция для сериализации (записи) объекта Date в файл
void serializeDate(const Date& date, const std::string& filename) {
    std::ofstream ofs(filename);
    if (!ofs) {
        std::cerr << "Ошибка открытия файла для записи.\n";
        return;
    }
    ofs << date;
    ofs.close();
}

// Функция для десериализации (чтения) объекта Date из файла
Date deserializeDate(const std::string& filename) {
    std::ifstream ifs(filename);
    if (!ifs) {
        throw "Ошибка открытия файла для чтения.";
    }
    Date date;
    ifs >> date;
    if (ifs.fail()) {
        throw "Ошибка чтения данных из файла.";
    }
    ifs.close();
    return date;
}

int main() {
    try {
        // Создание объекта Date
        Date d1(15, 5, 2025);
        std::cout << "Исходная дата: " << d1 << std::endl;

        // Сериализация объекта в файл "date.txt"
        serializeDate(d1, "date.txt");
        std::cout << "Дата успешно сериализована в файл 'date.txt'.\n";

        // Десериализация объекта из файла "date.txt"
        Date d2 = deserializeDate("date.txt");
        std::cout << "Десериализованная дата: " << d2 << std::endl;
    }
    catch (const char* error) {
        std::cerr << "Ошибка: " << error << std::endl;
        return 1;
    }
    return 0;
}
