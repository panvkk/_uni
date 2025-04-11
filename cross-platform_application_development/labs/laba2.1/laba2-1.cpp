#include <iostream>
#include <sstream>
#include <iomanip>
#include <string>
#include <ctime>
#include <chrono>
#include <map>

enum class DateFormat { STANDARD = 1, VERBAL = 2, AMERICAN = 3 };

class Date {
public:
    int day, month, year; 
    static DateFormat format; 

    Date() : day(1), month(1), year(1) {}
    Date(int d, int m, int y) : day(d), month(m), year(y) {}

    bool isValid() const {
        if (month < 1 || month > 12)
            return false;
        if (day < 1 || day > daysInMonth())
            return false;
        return true;
    }

    static bool isLeap(int y) {
        return (y % 400 == 0) || ((y % 4 == 0) && (y % 100 != 0));
    }

    int daysInMonth() const {
        static int mdays[13] = { 0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
        if (month == 2 && isLeap(year))
            return 29;
        return mdays[month];
    }

    int toJDN() const {
        int a = (14 - month) / 12;
        int y = year + 4800 - a;
        int m = month + 12 * a - 3;
        int jdn = day + (153 * m + 2) / 5 + 365 * y + y / 4 - y / 100 + y / 400 - 32045;
        return jdn;
    }

    static Date fromJDN(int jdn) {
        int a = jdn + 32044;
        int b = (4 * a + 3) / 146097;
        int c = a - (146097 * b) / 4;
        int d = (4 * c + 3) / 1461;
        int e = c - (1461 * d) / 4;
        int m = (5 * e + 2) / 153;
        int day = e - (153 * m + 2) / 5 + 1;
        int month = m + 3 - 12 * (m / 10);
        int year = b * 100 + d - 4800 + (m / 10);
        return Date(day, month, year);
    }

    Date operator+(int days) const {
        int jdn = toJDN();
        jdn += days;
        return fromJDN(jdn);
    }

    Date operator-(int days) const {
        return (*this) + (-days);
    }

    int operator-(const Date& other) const {
        return this->toJDN() - other.toJDN();
    }

    static Date today() {
        std::time_t t = std::time(nullptr);
        std::tm* now = std::localtime(&t);
        return Date(now->tm_mday, now->tm_mon + 1, now->tm_year + 1900);
    }

    int daysLived() const {
        Date t = Date::today();
        return t - (*this);
    }

    friend std::istream& operator>>(std::istream& in, Date& d);
    friend std::ostream& operator<<(std::ostream& out, const Date& d);
};

DateFormat Date::format = DateFormat::STANDARD;

std::string dayToOrdinal(int day) {
    switch(day) {
        case 1: return "первое";
        case 2: return "второе";
        case 3: return "третье";
        case 4: return "четвертое";
        case 5: return "пятое";
        case 6: return "шестое";
        case 7: return "седьмое";
        case 8: return "восьмое";
        case 9: return "девятое";
        case 10: return "десятое";
        case 11: return "одиннадцатое";
        case 12: return "двенадцатое";
        case 13: return "тринадцатое";
        case 14: return "четырнадцатое";
        case 15: return "пятнадцатое";
        case 16: return "шестнадцатое";
        case 17: return "семнадцатое";
        case 18: return "восемнадцатое";
        case 19: return "девятнадцатое";
        case 20: return "двадцатое";
        case 21: return "двадцать первое";
        case 22: return "двадцать второе";
        case 23: return "двадцать третье";
        case 24: return "двадцать четвертое";
        case 25: return "двадцать пятое";
        case 26: return "двадцать шестое";
        case 27: return "двадцать седьмое";
        case 28: return "двадцать восьмое";
        case 29: return "двадцать девятое";
        case 30: return "тридцатое";
        case 31: return "тридцать первое";
        default: return std::to_string(day);
    }
}

std::string monthToText(int month) {
    switch(month) {
        case 1: return "января";
        case 2: return "февраля";
        case 3: return "марта";
        case 4: return "апреля";
        case 5: return "мая";
        case 6: return "июня";
        case 7: return "июля";
        case 8: return "августа";
        case 9: return "сентября";
        case 10: return "октября";
        case 11: return "ноября";
        case 12: return "декабря";
        default: return "";
    }
}

std::istream& operator>>(std::istream& in, Date& d) {
    if (Date::format == DateFormat::STANDARD) {
        char dot1, dot2;
        in >> d.day >> dot1 >> d.month >> dot2 >> d.year;
        if (dot1 != '.' || dot2 != '.')
            in.setstate(std::ios::failbit);
    }
    else if (Date::format == DateFormat::AMERICAN) {
        char dot1, dot2;
        in >> d.year >> dot1 >> d.month >> dot2 >> d.day;
        if (dot1 != '.' || dot2 != '.')
            in.setstate(std::ios::failbit);
    }
    else if (Date::format == DateFormat::VERBAL) {
        std::string dayStr, monthStr, rest;
        in >> dayStr >> monthStr;
        std::getline(in, rest); 
        std::istringstream iss(rest);
        int yearNumber = 0;
        while (!iss.eof()) {
            std::string token;
            iss >> token;
            try {
                yearNumber = std::stoi(token);
                break;
            } catch (...) { }
        }
        std::map<std::string, int> dayMap = {
            {"первое", 1}, {"второе", 2}, {"третье", 3}, {"четвертое", 4},
            {"пятое", 5}, {"шестое", 6}, {"седьмое", 7}, {"восьмое", 8},
            {"девятое", 9}, {"десятое", 10}, {"одиннадцатое", 11}, {"двенадцатое", 12},
            {"тринадцатое", 13}, {"четырнадцатое", 14}, {"пятнадцатое", 15},
            {"шестнадцатое", 16}, {"семнадцатое", 17}, {"восемнадцатое", 18},
            {"девятнадцатое", 19}, {"двадцатое", 20}, {"двадцать первое", 21},
            {"двадцать второе", 22}, {"двадцать третье", 23}, {"двадцать четвертое", 24},
            {"двадцать пятое", 25}, {"двадцать шестое", 26}, {"двадцать седьмое", 27},
            {"двадцать восьмое", 28}, {"двадцать девятое", 29}, {"тридцатое", 30},
            {"тридцать первое", 31}
        };
        if (dayMap.find(dayStr) != dayMap.end())
            d.day = dayMap[dayStr];
        else
            in.setstate(std::ios::failbit);
        
        std::map<std::string, int> monthMap = {
            {"января", 1}, {"февраля", 2}, {"марта", 3}, {"апреля", 4},
            {"мая", 5}, {"июня", 6}, {"июля", 7}, {"августа", 8},
            {"сентября", 9}, {"октября", 10}, {"ноября", 11}, {"декабря", 12}
        };
        if (monthMap.find(monthStr) != monthMap.end())
            d.month = monthMap[monthStr];
        else
            in.setstate(std::ios::failbit);
        
        d.year = yearNumber;
    }
    if (!d.isValid())
        in.setstate(std::ios::failbit);
    return in;
}

std::ostream& operator<<(std::ostream& out, const Date& d) {
    if (Date::format == DateFormat::STANDARD) {
        out << (d.day < 10 ? "0" : "") << d.day << "."
            << (d.month < 10 ? "0" : "") << d.month << "."
            << d.year;
    }
    else if (Date::format == DateFormat::AMERICAN) {
        out << d.year << "."
            << (d.month < 10 ? "0" : "") << d.month << "."
            << (d.day < 10 ? "0" : "") << d.day;
    }
    else if (Date::format == DateFormat::VERBAL) {
        out << dayToOrdinal(d.day) << " " 
            << monthToText(d.month) << " ";
        if(d.year < 0)
            out << -d.year << " года до н.э.";
        else
            out << d.year << " год";
    }
    return out;
}

int main(){
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::cout << "Выберите формат ввода/вывода даты:\n";
    std::cout << "1 - DD.MM.YYYY (стандартный формат)\n";
    std::cout << "2 - Словесный формат\n";
    std::cout << "3 - YYYY.MM.DD (американский формат)\n";
    int choice;
    std::cin >> choice;
    if(choice < 1 || choice > 3) {
        std::cerr << "Неверный выбор формата.\n";
        return 1;
    }
    Date::format = static_cast<DateFormat>(choice);

    Date d;
    std::cout << "Введите дату: ";
    std::cin >> d;
    if(!std::cin) {
        std::cerr << "Ошибка ввода или некорректная дата!\n";
        return 1;
    }
    
    std::cout << "Введенная дата: " << d << "\n";

    std::cout << "Введите количество дней для прибавления: ";
    int addDays;
    std::cin >> addDays;
    Date newDate = d + addDays;
    std::cout << "Новая дата (дата + " << addDays << "): " << newDate << "\n";

    Date current = Date::today();
    std::cout << "Сегодня: " << current << "\n";
    int diffDays = current - d;
    std::cout << "Разница в днях между сегодняшней датой и введенной: " << diffDays << "\n";
    
    Date subDate = d - addDays;
    std::cout << "Дата (дата - " << addDays << "): " << subDate << "\n";
    
    std::cout << "Если введенная дата – ваша дата рождения, то прожито дней: " << d.daysLived() << "\n";

    system("pause");
    return 0;
}