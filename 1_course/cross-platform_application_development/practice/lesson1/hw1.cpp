#include <iostream>

struct node {
    int info;
    node* ssilka;
};

int main() {
    node* head;                     // Определяется переменна типа ссылки на структуру node, которая позже будет выступать в качестве головы списка

    head = new node;                // Создание 1 элемента
    std::cin >> head->info;
    head->ssilka = NULL;
    head->ssilka = new node;        // Создание 2 элемента
    std::cin >> head->ssilka->info;
    head->ssilka->ssilka = NULL;

    if(head->info > head->ssilka->info) {               // Сортировка 2-х элементов значения типа список
        std::swap(head->info, head->ssilka->info);
    }
    head->ssilka->ssilka = new node;                    // Создание 3 элемента
    std::cin >> head->ssilka->ssilka->info;
    head->ssilka->ssilka->ssilka = NULL;

    if(head->info > head->ssilka->ssilka->info) {                   // Сортировка всех 3-х элементов значения типа список 
        std::swap(head->info, head->ssilka->ssilka->info);          // Если 1 эл > 3 эл, то меняются местами сначала 1 и 3, затем 2 и 3 
        std::swap(head->ssilka->info, head->ssilka->ssilka->info);
    } else if(head->ssilka->info > head->ssilka->ssilka->info) {    // Если 2 эл > 3 эл, то меняются они меняются местами
        std::swap(head->ssilka->info, head->ssilka->ssilka->info);
    };

    std::cout << "result: \n";                                      // Вывод результатов
    std::cout << "first: " << head->info << ", adress: "<< head <<std::endl
    << "second: " << head->ssilka->info << ", adress: "<< head->ssilka <<std::endl
    << "third: " << head->ssilka->ssilka->info << ", adress: "<< head->ssilka->ssilka <<std::endl;
    return 0;
} 