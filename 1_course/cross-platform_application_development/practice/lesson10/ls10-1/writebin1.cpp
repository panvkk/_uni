#include <iostream>
#include <fstream>
#include <cstring>

// Отключаем выравнивание структуры, чтобы итоговый размер был ровно 53 байта.
#pragma pack(push, 1)
struct StudentRecord {
    char fio[50];         // ФИО занимает 50 байт (включая завершающий ноль)
    unsigned char day;    // День рождения (1 байт)
    unsigned char month;  // Месяц рождения (1 байт)
    unsigned char year;   // Год рождения (1 байт). Например, для 2007 записываем (2007 - 2000) = 7.
};
#pragma pack(pop)

int main() {
    // Открываем текстовый файл для чтения данных студентов.
    std::ifstream in("students.txt");
    if (!in) {
        std::cerr << "Ошибка открытия файла students.txt для чтения." << std::endl;
        return 1;
    }
    
    // Открываем бинарный файл для записи
    std::ofstream out("students.bin", std::ios::binary);
    if (!out) {
        std::cerr << "Ошибка открытия файла students.bin для записи." << std::endl;
        return 1;
    }
    
    // Переменные для считывания данных. Предполагается, что
    // в каждой строке файла: Фамилия Имя Отчество День Месяц Год extra
    std::string surname, name, patronymic;
    int day, month, year, extra;
    
    // Чтение каждой записи из текстового файла
    while (in >> surname >> name >> patronymic >> day >> month >> year >> extra) {
        StudentRecord record;
        // Объединяем ФИО в одну строку через пробелы
        std::string fioCombined = surname + " " + name + " " + patronymic;
        // Обнуляем массив, чтобы гарантировать отсутствие лишних символов
        std::memset(record.fio, 0, sizeof(record.fio));
        // Копируем не более 49 символов, чтобы оставить место для символа '\0'
        std::strncpy(record.fio, fioCombined.c_str(), sizeof(record.fio) - 1);
        
        record.day   = static_cast<unsigned char>(day);
        record.month = static_cast<unsigned char>(month);
        // Год записываем как смещение от 2000 (если год >= 2000)
        if (year >= 2000)
            record.year = static_cast<unsigned char>(year - 2000);
        else
            record.year = static_cast<unsigned char>(year);
        
        // Записываем запись в бинарный файл
        out.write(reinterpret_cast<const char*>(&record), sizeof(record));
    }
    
    in.close();
    out.close();
    
    std::cout << "Данные успешно записаны в файл students.bin." << std::endl;
    return 0;
}