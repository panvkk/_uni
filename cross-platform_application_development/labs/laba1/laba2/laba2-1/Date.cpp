#include "Date.h"
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

// Конструкторы
Date::Date() : d(0), m(0), y(0) {
    cout << *this << " - родился.\n";
}

Date::Date(int d, int m, int y) : d(d), m(m), y(y) {
    if (y > 0) {
        if (1 <= m && m <= 12) {
            if (!(1 <= d && d <= day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear()))) {
                throw "Ошибка. День введён неверно.";
            }
        }
        else {
            throw "Ошибка. Месяц должен быть в пределах [1..12].";
        }
    }
    else {
        throw "Ошибка. Год должен быть больше нуля.";
    }
}

Date::Date(const Date& rhs) : d(rhs.d), m(rhs.m), y(rhs.y) {}

// Арифметические операторы
Date Date::operator+(int day) const {
    if (day < 0) return *this - (-day);
    Date NewDate(*this);
    while (NewDate.AddYear(day));
    while (NewDate.AddMonth(day));
    while (NewDate.AddDay(day));
    return NewDate;
}

Date Date::operator-(int day) const {
    if (day < 0) return *this + (-day);
    Date NewDate(*this);
    while (NewDate.SubYear(day));
    while (NewDate.SubMonth(day));
    while (NewDate.SubDay(day));
    return NewDate;
}

int Date::operator-(const Date& rhs) const {
    return this->GetFromBegin() - rhs.GetFromBegin();
}

Date& Date::operator+=(int rhs) {
    *this = *this + rhs;
    return *this;
}

Date& Date::operator-=(int rhs) {
    *this = *this - rhs;
    return *this;
}

// Сравнительные операторы
bool Date::operator!=(const Date& rhs) const {
    return !(*this == rhs);
}

bool Date::operator==(const Date& rhs) const {
    return (this->d == rhs.d) && (this->m == rhs.m) && (this->y == rhs.y);
}

bool Date::operator<(const Date& rhs) const {
    if (this->y == rhs.y) {
        if (this->m == rhs.m) {
            return this->d < rhs.d;
        }
        else {
            return this->m < rhs.m;
        }
    }
    else {
        return this->y < rhs.y;
    }
}

bool Date::operator<=(const Date& rhs) const {
    return (*this == rhs) || (*this < rhs);
}

bool Date::operator>(const Date& rhs) const {
    if (this->y == rhs.y) {
        if (this->m == rhs.m) {
            return this->d > rhs.d;
        }
        else {
            return this->m > rhs.m;
        }
    }
    else {
        return this->y > rhs.y;
    }
}

bool Date::operator>=(const Date& rhs) const {
    return (*this == rhs) || (*this > rhs);
}

// Оператор присваивания
Date& Date::operator=(const Date& rhs) {
    if (this != &rhs) {
        this->d = rhs.d;
        this->m = rhs.m;
        this->y = rhs.y;
    }
    return *this;
}

// Деструктор
Date::~Date() {
    // cout << *this << " - убит.\n";
}

// Вспомогательные методы
int Date::GetFromBegin() const {
    int days = 0;
    for (int i = 1; i < this->y; i++) {
        days += 365 + static_cast<int>(IsLeapYear(i));
    }
    for (int i = 1; i < this->m; i++) {
        days += day_count[i - 1] + static_cast<int>((i == 2) && IsLeapYear());
    }
    days += d;
    return days;
}

Date Date::LocalTime() {
    time_t rawtime;
    tm* Time = new tm;
    time(&rawtime);
    localtime_s(Time, &rawtime);
    Date result(Time->tm_mday, Time->tm_mon + 1, Time->tm_year + 1900);
    delete Time;
    return result;
}

bool Date::AddDay(int& day) {
    if (day > 0) {
        day--;
        int day_cnt_in_this_month = day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear());
        d++;
        if (d > day_cnt_in_this_month) {
            d = 1;
            m++;
        }
        if (m > 12) {
            m = 1;
            y++;
        }
        return true;
    }
    return false;
}

bool Date::AddMonth(int& day) {
    while (d != 1 && AddDay(day));
    int day_cnt_in_this_month = day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear());
    if (day >= day_cnt_in_this_month) {
        day -= day_cnt_in_this_month;
        m++;
        if (m > 12) {
            m = 1;
            y++;
        }
        return true;
    }
    return false;
}

bool Date::AddYear(int& day) {
    while (d != 1 && AddDay(day));
    while (m != 1 && AddMonth(day));
    int day_in_this_year = 365 + static_cast<int>(IsLeapYear());
    if (day >= day_in_this_year) {
        day -= day_in_this_year;
        y++;
        return true;
    }
    return false;
}

bool Date::SubYear(int& day) {
    while (d != day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear()) && SubDay(day));
    while (m != 12 && SubMonth(day));
    int day_in_this_year = 365 + static_cast<int>(IsLeapYear());
    if (day >= day_in_this_year) {
        day -= day_in_this_year;
        y--;
        return true;
    }
    return false;
}

bool Date::SubMonth(int& day) {
    while (d != day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear()) && SubDay(day));
    int day_cnt_in_this_month = day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear());
    if (day >= day_cnt_in_this_month) {
        day -= day_cnt_in_this_month;
        m--;
        if (m < 1) {
            m = 12;
            y--;
        }
        d = day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear());
        return true;
    }
    return false;
}

bool Date::SubDay(int& day) {
    if (day > 0) {
        day--;
        d--;
        if (d < 1) {
            m--;
            if (m < 1) {
                m = 12;
                y--;
            }
            int day_cnt_in_month = day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear());
            d = day_cnt_in_month;
        }
        return true;
    }
    return false;
}

bool Date::IsLeapYear(int year) const {
    if (year == -1) {
        year = this->y;
    }
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    if (year % 4 == 0) return true;
    return false;
}

// Перегрузка оператора вывода
ostream& operator<<(ostream& outp__, const Date& date__) {
    outp__ << setw(2) << setfill('0') << date__.d << ".";
    outp__ << setw(2) << setfill('0') << date__.m << ".";
    outp__ << setw(4) << setfill('0') << date__.y;
    return outp__;
}

void operator>>(const Date& date__, Date& inp__) {
    date__
}
