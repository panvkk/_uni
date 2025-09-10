#include <iostream>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <string>

class Date {
public:
    Date();
    Date(int d, int m, int y);
    Date(const Date& rhs);

    static Date LocalTime();

    friend std::ostream& operator<<(std::ostream& out, const Date& date);
    friend std::istream& operator>>(std::istream& in, Date& date);

    Date operator+(int rhs) const;
    Date operator-(int rhs) const;
    int operator-(const Date& rhs) const;

    Date& operator+=(int rhs);
    Date& operator-=(int rhs);

    bool operator!=(const Date& rhs) const;
    bool operator==(const Date& rhs) const;
    bool operator<(const Date& rhs) const;
    bool operator<=(const Date& rhs) const;
    bool operator>(const Date& rhs) const;
    bool operator>=(const Date& rhs) const;

    Date& operator=(const Date& rhs);

    void writeBinary(std::ostream& os) const;
    void readBinary(std::istream& is);

    ~Date();

private:
    int GetFromBegin() const;
    bool AddDay(int& day);
    bool AddMonth(int& day);
    bool AddYear(int& day);
    bool SubYear(int& day);
    bool SubMonth(int& day);
    bool SubDay(int& day);
    bool IsLeapYear(int year = -1) const;

    const unsigned char day_count[12] = { 31,28,31,30,31,30,31,31,30,31,30,31 };

    unsigned char d;
    unsigned char m;
    unsigned short y;
};

// ---------------- Реализация методов класса Date ----------------

Date::Date() : d(0), m(0), y(0) {
    std::cout << *this << " - создан объект Date.\n";
}

Date::Date(int d, int m, int y) {
    if (y < 0)
        throw "Ошибка. Год должен быть положительным.";
    if (m < 1 || m > 12)
        throw "Ошибка. Месяц должен быть в пределах [1..12].";
    int max_day = day_count[m - 1] + static_cast<int>((m == 2) && IsLeapYear(y));
    if (!(d >= 1 && d <= max_day))
        throw "Ошибка. День введён неверно.";
    this->d = static_cast<unsigned char>(d);
    this->m = static_cast<unsigned char>(m);
    this->y = static_cast<unsigned short>(y);
}

Date::Date(const Date& rhs) : d(rhs.d), m(rhs.m), y(rhs.y) { }

Date::~Date() { }

Date Date::LocalTime() {
    time_t rawtime;
    tm TimeStruct;
    time(&rawtime);
#ifdef _WIN32
    localtime_s(&TimeStruct, &rawtime);
#else
    localtime_r(&rawtime, &TimeStruct);
#endif
    return Date(TimeStruct.tm_mday, TimeStruct.tm_mon + 1, TimeStruct.tm_year + 1900);
}

Date Date::operator+(int day) const {
    if (day < 0)
        return *this - (-day);
    Date NewDate(*this);
    while (day > 0) {
        NewDate.d++;
        int max_day = NewDate.day_count[NewDate.m - 1] +
                      static_cast<int>((NewDate.m == 2) && NewDate.IsLeapYear());
        if (NewDate.d > max_day) {
            NewDate.d = 1;
            NewDate.m++;
            if (NewDate.m > 12) {
                NewDate.m = 1;
                NewDate.y++;
            }
        }
        day--;
    }
    return NewDate;
}

Date Date::operator-(int day) const {
    if (day < 0)
        return *this + (-day);
    Date NewDate(*this);
    while (day > 0) {
        NewDate.d--;
        if (NewDate.d < 1) {
            NewDate.m--;
            if (NewDate.m < 1) {
                NewDate.m = 12;
                NewDate.y--;
            }
            NewDate.d = NewDate.day_count[NewDate.m - 1] +
                        static_cast<int>((NewDate.m == 2) && NewDate.IsLeapYear());
        }
        day--;
    }
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

bool Date::operator==(const Date& rhs) const {
    return (d == rhs.d && m == rhs.m && y == rhs.y);
}

bool Date::operator!=(const Date& rhs) const {
    return !(*this == rhs);
}

bool Date::operator<(const Date& rhs) const {
    if (y != rhs.y) return y < rhs.y;
    if (m != rhs.m) return m < rhs.m;
    return d < rhs.d;
}

bool Date::operator<=(const Date& rhs) const {
    return (*this < rhs) || (*this == rhs);
}

bool Date::operator>(const Date& rhs) const {
    return !(*this <= rhs);
}

bool Date::operator>=(const Date& rhs) const {
    return !(*this < rhs);
}

Date& Date::operator=(const Date& rhs) {
    if (this != &rhs) {
        d = rhs.d;
        m = rhs.m;
        y = rhs.y;
    }
    return *this;
}

int Date::GetFromBegin() const {
    int days = 0;
    for (int i = 1; i < static_cast<int>(y); i++) {
        days += 365 + static_cast<int>((i % 400 == 0) || ((i % 4 == 0) && (i % 100 != 0)));
    }
    for (int i = 1; i < static_cast<int>(m); i++) {
        days += day_count[i - 1] + static_cast<int>((i == 2) && IsLeapYear(y));
    }
    days += d;
    return days;
}

bool Date::AddDay(int& /*day*/) { return false; }
bool Date::AddMonth(int& /*day*/) { return false; }
bool Date::AddYear(int& /*day*/) { return false; }
bool Date::SubYear(int& /*day*/) { return false; }
bool Date::SubMonth(int& /*day*/) { return false; }
bool Date::SubDay(int& /*day*/) { return false; }

bool Date::IsLeapYear(int year) const {
    if (year == -1)
        year = y;
    if (year % 400 == 0) return true;
    if (year % 100 == 0) return false;
    if (year % 4 == 0) return true;
    return false;
}

std::ostream& operator<<(std::ostream& out, const Date& date) {
    out << std::setw(2) << std::setfill('0') << static_cast<int>(date.d) << "."
        << std::setw(2) << std::setfill('0') << static_cast<int>(date.m) << "."
        << std::setw(4) << std::setfill('0') << static_cast<int>(date.y);
    return out;
}

std::istream& operator>>(std::istream& in, Date& date) {
    int day, month, year;
    char sep1, sep2;
    if (in >> day >> sep1 >> month >> sep2 >> year) {
        if (sep1 == '.' && sep2 == '.') {
            try {
                date = Date(day, month, year);
            }
            catch (const char* error) {
                std::cerr << "Ошибка десериализации: " << error << std::endl;
                in.setstate(std::ios::failbit);
            }
        } else {
            in.setstate(std::ios::failbit);
        }
    }
    return in;
}

// ---------------- Методы бинарной сериализации/десериализации ----------------

void Date::writeBinary(std::ostream& os) const {
    os.write(reinterpret_cast<const char*>(&d), sizeof(d)); // 1 байт
    os.write(reinterpret_cast<const char*>(&m), sizeof(m)); // 1 байт
    os.write(reinterpret_cast<const char*>(&y), sizeof(y)); // 2 байта
}

void Date::readBinary(std::istream& is) {
    is.read(reinterpret_cast<char*>(&d), sizeof(d));
    is.read(reinterpret_cast<char*>(&m), sizeof(m));
    is.read(reinterpret_cast<char*>(&y), sizeof(y));
}

// ---------------- Функции бинарной сериализации/десериализации ----------------

void serializeDate(const Date& date, const std::string& filename) {
    std::ofstream ofs(filename, std::ios::binary);
    if (!ofs) {
        std::cerr << "Ошибка открытия файла для записи.\n";
        return;
    }
    date.writeBinary(ofs);
    ofs.close();
}

Date deserializeDate(const std::string& filename) {
    std::ifstream ifs(filename, std::ios::binary);
    if (!ifs) {
        throw "Ошибка открытия файла для чтения.";
    }
    Date date;
    date.readBinary(ifs);
    if (ifs.fail()) {
        throw "Ошибка чтения данных из файла.";
    }
    ifs.close();
    return date;
}

// ---------------- main ----------------

int main() {
    try {
        Date d1;
        std::cout << "Введите исходную дату (в формате dd.mm.yyyy): ";
        std::cin >> d1;
        
        serializeDate(d1, "date.bin");
        std::cout << "Дата успешно сериализована в бинарный файл 'date.bin'.\n";
        
        Date d2 = deserializeDate("date.bin");
        std::cout << "Десериализованная дата: " << d2 << std::endl;
    }
    catch (const char* error) {
        std::cerr << "Ошибка: " << error << std::endl;
        return 1;
    }
    return 0;
}
