#include <iostream> #include <Windows.h> #include "Date.h" using namespace std;
int main() { SetConsoleOutputCP(1251); try { cout << "За бортом -> " <<
            Date::LocalTime() << ".\n"; Date Puhkin_B(6, 6, 1799); Date Puhkin_D(10, 2, 1873);
        Date Lenin(22, 4, 1870); Date Stalin(18, 12, 1878); cout << "Тем временем В.И.
                Ленин родился " << Lenin << ".\n"; cout << "Это было " << Date::LocalTime() -
                Lenin << " дней назад.\n"; cout << "Спустя некоторое время родился И.В.
            Сталин, а именно " << Stalin << ".\n"; cout << "Это произошло спустя " << Stalin
                    - Lenin << " дней после рождения В.И. Ленина.\n"; cout << "Однако мы всего в "
             << Date::LocalTime() - Stalin << " днях от рождения Сталин.\n"; cout <<
            "Интересный факт: Пушкин родился " << Puhkin_B << ". И прожил всего " <<
            Puhkin_D - Puhkin_B << ".\n"; cout << "А мы без пушкина уже " <<
            Date::LocalTime() - Puhkin_D << " дней.\n"; cout << "Завтра " <<
            Date::LocalTime() + 1 << ".\n"; cout << "Через 10 дней будет " <<
            Date::LocalTime() + 10 << ".\n"; cout << "Через 1000 дней будет " <<
            Date::LocalTime() + 1000 << ".\n"; } catch (exception e) { cout << e.what() << '\n'; }
    system("pause"); }
