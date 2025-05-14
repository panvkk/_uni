#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>

#define MAX_LEN 100  

struct Student {
    char fullname[MAX_LEN];
    char currentPlace[30];  
    char registration[30]; 
    short int year;                    
    unsigned char month;                  
    unsigned char day;                   
    unsigned short int admissionScore;        
};

int main() {
    struct Student students[10], open_student;
    
    std::ifstream input("Input.txt");

    int groupCount = 0;
    std::string line;
    while(groupCount < 10 && std::getline(input, line)) {
        strncpy(students[groupCount].fullname, line.c_str(), MAX_LEN - 1);
        // line.erase(line.begin(), line.begin() + MAX_LEN + 1);
        students[groupCount].fullname[MAX_LEN - 1] = '\0';
        
        if(!std::getline(input, line)) break;
        strncpy(students[groupCount].currentPlace, line.c_str(), 29);
        // line.erase(line.begin(), line.begin() + 29);
        students[groupCount].currentPlace[29] = '\0';
        
        if(!std::getline(input, line)) break;
        strncpy(students[groupCount].registration, line.c_str(), 29);
        // line.erase(line.begin(), line.begin() + 29);
        students[groupCount].registration[29] = '\0';
        
        if(!std::getline(input, line)) break;
        students[groupCount].year = static_cast<short int>(std::stoi(line));
        
        if(!std::getline(input, line)) break;
        students[groupCount].month = static_cast<unsigned char>(std::stoi(line));
        
        if(!std::getline(input, line)) break;
        students[groupCount].day = static_cast<unsigned char>(std::stoi(line));
        
        if(!std::getline(input, line)) break;
        students[groupCount].admissionScore = static_cast<unsigned short int>(std::stoi(line));
        
        groupCount++;
    }
    input.close();

    FILE* file;
    file = fopen("D:\\_uni\\cross-platform_application_development\\practice\\lesson10\\ls10-1\\students.info", "wb");

    for(int i = 0; i < 10; i++)
        fwrite(&students[i], sizeof(struct Student), 1, file);

    printf("Данные успешно записаны в файл students.info\n");
    
    if (fread(&open_student, sizeof(struct Student), 1, file) != 1) {
        perror("Ошибка при чтении файла");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    
    int outputCount = 0;

    while(outputCount <= 10) {
        struct Student open_student = students[outputCount];

        printf("\n%d\n", outputCount);
        printf("Данные студента из файла student.info:\n", ", длина в байтах - ", sizeof(open_student.fullname));
        printf("ФИО: %s\n", open_student.fullname, ", длина в байтах - ", sizeof(open_student.fullname));
        printf("Место рождения: %s\n", open_student.currentPlace, ", длина в байтах - ", sizeof(open_student.fullname));
        printf("Прописка: %s\n", open_student.registration, ", длина в байтах - ", sizeof(open_student.fullname));
        printf("Дата рождения: %02hhu.%02hhu.%hd\n", open_student.day, open_student.month, open_student.year);
        printf("Балл при поступлении: %hu\n", open_student.admissionScore, ", длина в байтах - ", sizeof(open_student.fullname));
        
        outputCount++;
    }

    fclose(file);

    system("pause");
    return 0;
}
