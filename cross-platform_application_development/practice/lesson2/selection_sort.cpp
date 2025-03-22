#include <iostream>

struct node {
    int info;
    node* ssylka;
};

int main() {
    setlocale(LC_ALL, "Russian");
    node* head; 
    node* work; 
    node* curr;
    node* curr_in_sortion;
    int sum, count;
    int min_el, max_el;
    node* adr_min_el;
    node* adr_max_el;
    node* pred;
    bool induk;

    // Приветствие
    std::cout << "Добрый день, дедушка. Данная программа позволяет Вам ввести,"
     << " набрав на клавиатуре, последовательность чисел,\nоканчивающуюся нулём, после чего будет создан список в оперативной памяти, и на экран\n"
     << "будет выведена информационная и адресная часть каждого элемента списка.\nВводить числа можно через пробел или каждое число с новой строки(нажимая клавишу Ввод).\n";

    head = new node;
    std::cin >> head->info;
    curr = head;
    while(curr->info) {
        work = new node;
        std::cin >> work->info;
        work->ssylka = NULL;
        curr->ssylka = work;
        curr = work;
    }

    std::cout << "\nЭлементы списка, введённого с помощью клавиатуры. \n";
    curr = head; // Подготовка цикла.
    if(head->info == 0) {              // Проверка списка на пустоту
        std::cout << "Дедушка, Вы редиска! Список пуст!\n";
        system("pause");
    } 
    count = 1;
    while(curr->ssylka) {
        std::cout << count << " элемент списочной структуры: " << "информационная часть - " << curr->info << ", адресная часть - " << curr->ssylka << std::endl; // - Это буква В
        count = count + 1;
        curr = curr->ssylka;
    }

    
    curr = head; // Подготовка цикла 
    sum = 0;
    count = 0;
    while(curr->ssylka != NULL) {       // Нахождение среднего арифметического
        sum = sum + curr->info;
        count = count + 1;
        curr = curr->ssylka;
    }
    std::cout << "\nСреднее арифметическое выведенных на экран ранее чисел равно: " << sum/count << " - целых, " << sum%count << " - в остатке" << std::endl;


    curr = head; 
    min_el = head->info;
    max_el = head->info;
    adr_min_el = head;
    adr_max_el = head;

    while(curr->ssylka != NULL) {           // Нахождение максимального и минимального элементов
        if(min_el > curr->info) { 
            min_el = curr->info; 
            adr_min_el = curr;
        } 
        else if(max_el < curr->info) { 
            max_el = curr->info; 
            adr_max_el = curr;
        }
        curr = curr->ssylka;
    }
    std::swap(adr_max_el->info, adr_min_el->info);     // Меняю местами максимальный и минимальный элемент

    curr = head;
    std::cout << "\nСписок, в котором максимальное и минимальное значения поменены местами: \n";
    count = 1;
    while(curr->ssylka != NULL) {
        std::cout << count << " элемент списочной структуры: " << "информационная часть - " << curr->info << ", адресная часть - " << curr->ssylka << std::endl; // - Это буква В
        count = count + 1;
        curr = curr->ssylka;
    }  
    /*
    Задача. 
    Удалить из полученного списка максимальный и минимальный элемент. Полученный список вывести на экран
    */
    curr = head;
    pred = head;
    induk = true;
    
    if(head->info == min_el) {          // Если минимальный элемент находится в голове, то голова должна ссылаться на следующий элемент
        work = head;
        head = head->ssylka; 
        delete work;             // А сам минимальный удаляем
    } else
        while(induk)
            if(curr->info == min_el) {          // Удаляем минимальный элемент
                if(curr->ssylka != NULL) {  // Проверка, является ли минимальный элемент последним элементом списка
                    pred->ssylka = curr->ssylka;
                }    
                delete curr; 
                induk = false;
            } else {
                pred = curr;
                curr = curr->ssylka;
            }
        
    curr = head;
    pred = head;
    induk = true;
    
    if(head->info == max_el) {          // Если максимальный элемент находится в голове, то голова должна ссылаться на следующий
        work = head;
        head = head->ssylka; 
        delete work;            // А сам максимальный удаляем
    } else
        while(induk)                    
            if(curr->info == max_el) { 
                if(curr->ssylka != NULL) {  // Проверка, является ли максимальный элемент последним элементом списка
                    pred->ssylka = curr->ssylka;
                }             
                delete curr;            // Удаляем максимальный элемент
                induk = false;
            } else {
                pred = curr;
                curr = curr->ssylka;
            }
        
    curr = head; // Подготовка цикла.
    std::cout << "\nСписок, в котором минимальное и максимальное значения удалены: \n";
    count = 1;
    while(curr->ssylka != NULL) {
        std::cout << count << " элемент списочной структуры: " << "информационная часть - " << curr->info << ", адресная часть - " << curr->ssylka << std::endl; // - Это буква В
        count = count + 1;
        curr = curr->ssylka;
    }   

//##################################################################################################################

    // Сортировка списка по-неубыванию выбором

    curr = head;            // Подготовка цикла
    while(curr->ssylka) {
        min_el = curr->info;
        adr_min_el = curr;

        curr_in_sortion = curr;
        while(curr_in_sortion->ssylka) {           // Нахождение минимального элемента
            if(min_el > curr_in_sortion->info) { 
                min_el = curr_in_sortion->info; 
                adr_min_el = curr_in_sortion;
            }
            curr_in_sortion = curr_in_sortion->ssylka;
        }
        std::swap(adr_min_el->info, curr->info);            // Меняю местами минимальный найденный в списке элемент с текущим
        curr = curr->ssylka;                        
    }


    curr = head; // Подготовка цикла.
    std::cout << "\nОтсортированный по-неубыванию список: \n";
    count = 1;
    while(curr->ssylka != NULL) {
        std::cout << count << " элемент списочной структуры: " << "информационная часть - " << curr->info << ", адресная часть - " << curr->ssylka << std::endl; // - Это буква В
        count = count + 1;
        curr = curr->ssylka;
    }  
    system("pause");
}