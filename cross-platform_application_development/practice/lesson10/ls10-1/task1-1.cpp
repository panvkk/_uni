#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    struct Student student, open_student;
    
    FILE* file;
    file = fopen("D:\\_uni\\cross-platform_application_development\\practice\\lesson10\\ls10-1\\student.info", "rb");

    // printf("Введите ФИО (Фамилия Имя Отчество): ");
    // fgets(student.fullname, MAX_LEN, stdin);
    
    // size_t len = strlen(student.fullname);
    // if (len > 0 && student.fullname[len - 1] == '\n') {
    //     student.fullname[len - 1] = '\0';
    // }

    // printf("Введите место рождения: ");
    // scanf("%30s", student.currentPlace);

    // printf("Введите прописку: ");
    // scanf("%30s", student.registration);

    // printf("Введите год рождения: ");
    // scanf("%hd", &student.year);

    // printf("Введите месяц рождения (1-12): ");
    // scanf("%hhu", &student.month);

    // printf("Введите день рождения (1-31): ");
    // scanf("%hhu", &student.day);

    // printf("Введите балл при поступлении (не более 400): ");
    // scanf("%hu", &student.admissionScore);

    // FILE* file;
    // file = fopen("D:\\_uni\\cross-platform_application_development\\practice\\lesson10\\ls10-1\\student.info", "wb");

    // fwrite(&student, sizeof(student), 1, file);

    // printf("Данные успешно записаны в файл student.info\n");
    
    if (fread(&open_student, sizeof(struct Student), 1, file) != 1) {
        perror("Ошибка при чтении файла");
        fclose(file);
        exit(EXIT_FAILURE);
    }

    fclose(file);
    
    printf("\nДанные студента из файла student.info:\n");
    printf("ФИО: %s\n", open_student.fullname);
    printf("Место рождения: %s\n", open_student.currentPlace);
    printf("Прописка: %s\n", open_student.registration);
    printf("Дата рождения: %02hhu.%02hhu.%hd\n", open_student.day, open_student.month, open_student.year);
    printf("Балл при поступлении: %hu\n", open_student.admissionScore);

    system("pause");
    return 0;
}
