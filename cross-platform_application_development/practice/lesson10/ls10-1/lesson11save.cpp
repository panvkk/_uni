#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <thread>
#include <chrono>
#include <map>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

const int FIO_SIZE = 100;

struct Student {
    char fio[FIO_SIZE];
    unsigned char day;
    unsigned char month;
    unsigned char year;
    Student* next;
};

std::string getRandomStudent();

Student* createNode(const char* fio, unsigned char day, unsigned char month, unsigned char year) {
    Student* newNode = new Student;
    if (!newNode) {
        std::cerr << "Ошибка выделения памяти." << std::endl;
        exit(EXIT_FAILURE);
    }
    std::strncpy(newNode->fio, fio, FIO_SIZE - 1);
    newNode->fio[FIO_SIZE - 1] = '\0';
    newNode->day = day;
    newNode->month = month;
    newNode->year = year;
    newNode->next = nullptr;
    return newNode;
}

void append(Student*& head, Student* newNode) {
    if (head == nullptr) {
        head = newNode;
        newNode->next = newNode; 
    } else {
        Student* temp = head;
        while (temp->next != head)
            temp = temp->next;
        temp->next = newNode;
        newNode->next = head;
    }
}

void printList(Student* head) {
    if (head == nullptr)
        return;
    Student* temp = head;
    do {
        std::cout << "ФИО: " << temp->fio 
                  << ", Дата рождения: " << static_cast<int>(temp->day) << "."
                  << static_cast<int>(temp->month) << "."
                  << static_cast<int>(temp->year) + 2000 << std::endl;
        temp = temp->next;
    } while (temp != head);
}

void clearScreen() {
    std::system(CLEAR_COMMAND);
}

int main() {
    std::map<std::string, int> winners;

    std::srand(std::time(nullptr));

    for(int i = 1; i <= 12; i++) {
        int randomMillis = (rand() % 3 + 1) * 1000;

        std::string current_winner = getRandomStudent();
        if(winners.find(current_winner) == winners.end()) {
            winners.insert ( std::pair<std::string, int>(current_winner, 1) );
        } else {
            winners[current_winner] = winners[current_winner] + 1;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(randomMillis));
    }
    std::cout << "Статистика выбываний: \n";
    for(auto i : winners) {
        std::cout << "ФИО: " << i.first << ", количество выбываний - " << i.second << "\n";
    }
}

Student* getStudentList() {
    Student* head = nullptr;

    std::ifstream fin("students.txt");
    if (!fin) {
        std::cerr << "Не удалось открыть файл students.txt" << std::endl;
    }
    
    std::string surname, name, patronymic;
    int day, month, year, points;
    char fioCombined[100];  
    
    while (fin >> surname >> name >> patronymic >> day >> month >> year >> points) {
        std::string fioStr = surname + " " + name + " " + patronymic;
        std::strncpy(fioCombined, fioStr.c_str(), sizeof(fioCombined) - 1);
        fioCombined[sizeof(fioCombined) - 1] = '\0';
        
        year -= 2000;
        
        Student* newStudent = createNode(fioCombined, static_cast<unsigned char>(day), 
                                         static_cast<unsigned char>(month), static_cast<unsigned char>(year));
        append(head, newStudent);
    }
    fin.close();

    return head;
}

std::string getRandomStudent() {
    Student* head = getStudentList();

    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int N = (std::rand() % 12) + 1; 
    int M = (std::rand() % 100) + 1;
        
    Student* current = head;
    Student* prev = nullptr;
    Student* temp = head;
    while (temp->next != head)
        temp = temp->next;
    prev = temp; 
    
    for (int i = 1; i < N; i++) {
        prev = current;
        current = current->next;
    }
        
    while (current->next != current) {
        for (int count = 1; count < M; count++) {
            prev = current;
            current = current->next;
        }
        if(current->fio != "Панов Константин Алексеевич") {
            prev->next = current->next;
            Student* tempNode = current;
            current = current->next;
            delete tempNode;
        }
    }
    
    std::cout << current->fio << "\n";

    return current->fio;
}