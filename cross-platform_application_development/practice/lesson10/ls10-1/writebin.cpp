#include <fstream>
#include <cstring>
#include <cstdlib>
#include <string>

struct Student {
    char fio[50];
    unsigned char day;
    unsigned char month;
    unsigned char year;
    int points;
};

int main() {
    std::ifstream txtFile("students.txt");
    FILE* binFile;
    binFile = fopen("D:\\_uni\\cross-platform_application_development\\practice\\lesson10\\ls10-1\\students.bin", "wb");

    std::string surname, name, otec;
    int day, month, year, points;
    while(txtFile >> surname >> name >> otec >> day >> month >> year >> points) {
        Student* current_student = new Student;

        char* fioCombined;
        std::string fioStr = surname + " " + name + " " + otec;
        std::strncpy(fioCombined, fioStr.c_str(), sizeof(fioCombined) - 1);
        fioCombined[sizeof(fioCombined) - 1] = '\0';

        year = year%10;

        std::strncpy(current_student->fio, fioCombined, 49);
        current_student->fio[49] = '\0';

        current_student->day = static_cast<unsigned char>(day);
        current_student->month = static_cast<unsigned char>(month);
        current_student->year = static_cast<unsigned char>(year);
        current_student->points = points;

        fwrite(&*current_student, sizeof(struct Student), 1, binFile);
    }
}