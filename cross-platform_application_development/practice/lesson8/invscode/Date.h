#pragma once
#include <iostream>
using namespace std;

// Класс для работы с датами
class Date {
public:
    // Конструкторы
    Date();
    Date(int d, int m, int y);
    Date(const Date& rhs);

    // Статический метод для получения локального времени
    static Date LocalTime();

    // Перегрузка оператора вывода
    friend ostream& operator<<(ostream& outp__, const Date& date__);

    // Арифметические операторы
    Date operator+(int rhs) const;
    Date operator-(int rhs) const;
    int operator-(const Date& rhs) const;

    Date& operator+=(int rhs);
    Date& operator-=(int rhs);

    // Сравнительные операторы
    bool operator!=(const Date& rhs) const;
    bool operator==(const Date& rhs) const;
    bool operator<(const Date& rhs) const;
    bool operator<=(const Date& rhs) const;
    bool operator>(const Date& rhs) const;
    bool operator>=(const Date& rhs) const;

    // Оператор присваивания
    Date& operator=(const Date& rhs);

    // Деструктор
    ~Date();

private:
    // Вспомогательные методы для работы с датой
    int GetFromBegin() const;
    bool AddDay(int& day);
    bool AddMonth(int& day);
    bool AddYear(int& day);
    bool SubYear(int& day);
    bool SubMonth(int& day);
    bool SubDay(int& day);
    bool IsLeapYear(int year = -1) const;

    // Массив количества дней в каждом месяце
    const unsigned char day_count[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

    // Поля для хранения дня, месяца и года
    int d, m, y;
};
