#include <iostream>
#include <fstream>
#include <cstring>
#include <cstdlib>
#include <ctime>

#ifdef _WIN32
    #define CLEAR_COMMAND "cls"
#else
    #define CLEAR_COMMAND "clear"
#endif

const int FIO_SIZE = 50;

struct Student {
    char fio[FIO_SIZE];
    unsigned char day;
    unsigned char month;
    unsigned char year;
    Student* next;
};

/*
Операция вывода информации на экран >> является переопределённой операцией от операции сдвига в том случае када справа от 
неё располагается символическое имя монитора(сщге) либо имя ткт файла 
1. Значение в переменной слева от >> преобразуется в последовательгость символов.
2. Последоват выводится на экран, при этом договорились разработчики ПС которые функц в рамках опер оболочек винда линекс .. 
что чсасть симовлов таблицы код будут исопльзоваться как управляющеие символы шкоаторыре вшоватмолаитвд при попбадуаов их в 
обраьроиут фукнци оявывыода на кран иил в ыв тетысый фалй не быодт иметь изоары яргрвя на эдррямик а выполрятья пнмвсеришрряи усаоф 
иывоаимвв в ва мячсианси омиваи моваимщорыиваооваш оиывар имырвдо 10 када  поаоаывоет врна ука тыоивоаршоы пщрыкео пиывоа ития т
ываргщщыгвапгывап щыгвщгвгщп на жкран е а дуеоте осущетсвлять управление курсовом а то ест курсов будет перевермн на следующейюущ шиов иоси мдоику
то жеопшок см аомоещ проиргзщшрмшоуика шсо имщукимщ руиамоуда с коолом рки цруаи 13 фозврат каретки в функцию больше больше 
символ не оторражается а курслор в начало строки более того вс операции вс и везде имеют проблемы с отображением символов в зав от 
используемой кодировки  оывтапдыивдапотывдо прв ироводлл ытваоывои риаориы оаылвапыаип и вапрос как бароца ед способо барбы с ывталыв
является есть согласованеи то есть да кодировок ага меньше информации больше слов вода вод вода кодировки должн да быть одина 
используют кодировку юникод а именнь кодировку в которй под кажюый символ 2 байта и так же как есть кодировочная таблица
сущ кодировка юникод где каждому символу ставится в соответсвие 2 байта то есть всего можно закодировать 65 536 различныых символов
в любой системе можно ща задать кодировку для программ. Посмотреть на содержимое бин файла то этот файл располагается на диске нужно
использовать соответствующую кодировку и не использовать юникод иметь в виоду что нек кодиров4как используется в вижле, в рамках
операц оолочке  виндус и при символы паказать на икране 

када \n и говорит 1 симовл то нет это фигня бред это 2 символа и это указание компилятору с языка С что при компиляции программы 
эти 2 символа должны бть преобразованы в символ с кодом 10 чобы >> увидела это символ с кодом 10 и что эта операция доллжна перевести
курсор на первую строку 
*/

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
                  << static_cast<int>(temp->month) << ".200"
                  << static_cast<int>(temp->year) << std::endl;
        temp = temp->next;
    } while (temp != head);
}

int main() {
    Student* head = nullptr;

    FILE* file;
    try {
        file = fopen("D:\\_uni\\cross-platform_application_development\\practice\\lesson10\\ls10-1\\students.bin", "rb");
    } catch(...) {
        std::cerr << "Ошибка при открытии бинарного файла.";
    }

    Student* current_student;

    while(fread(&*current_student, sizeof(struct Student), 1, file)) {
        append(head, current_student);
    }
    fclose(file);
    
    
    std::cout << "Исходный список студентов:\n" << std::endl;
    printList(head);
    
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    int N = (std::rand() % 12) + 1;  
    int M = (std::rand() % 100) + 1;
    
    std::cout << "\nСлучайные числа: N = " << N << ", M = " << M << "\n" << std::endl;
    
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
    
    std::cout << "Порядок исключения студентов:\n" << std::endl;
    
    while (current->next != current) {
        for (int count = 1; count < M; count++) {
            prev = current;
            current = current->next;
        }
        std::cout << current->fio << std::endl;
        prev->next = current->next;
        Student* tempNode = current;
        current = current->next;
        delete tempNode;
    }
        
    int consoleWidth = 80;
    int len = std::strlen(current->fio);
    int padding = (consoleWidth - len) / 2;
    if (padding < 0)
        padding = 0;
    
    for (int i = 0; i < 10; i++)
        std::cout << "\n";
    for (int i = 0; i < padding; i++)
        std::cout << " ";
    std::cout << current->fio << "\n";
    
    delete current;
    
    return 0;
}
