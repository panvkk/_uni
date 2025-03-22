/*
С клавиатуры вводится полсдеовательность чисел, которая оканчивается нулём(для желающих оканчивается вводом двух чисел 0 подряд).
Необходимо сформировать в оперативной памяти список(целые числа) таким образом, чтобы в результате получился отсортированный по
неубыванию список, то есть, каждый новый элемент добавляется в список в нужно место.
*/
#include <iostream>

struct node {
    float info;
    node* ssylka;
};

int main() {
    node* head;
    node* curr;
    node* work;
    node* pred;
    float sum;
    bool induk;


    head = new node;
    std::cin >> head->info;
    head->ssylka = NULL;

    work = new node;
    std::cin >> work->info;
    while(work->info) {
        /* 
            Удалить 2 операции: work = new node; , std::cin >> work->info; и эти 2 операции записать там, где записаны
        и вставляем после выполняемых действий.
        */
        induk = true;
        pred = NULL;
        curr = head;
        while(induk && curr) {
            if(curr->info >= work->info) {
                if(!pred) {
                    work->ssylka = curr;
                    curr = work;
                } else {
                    pred->ssylka = work;
                    work->ssylka = curr;
                }
                induk = false;
            } else {
                pred = curr;
                curr = curr->ssylka;
            }
        }
        if(induk == true) {
            pred->ssylka = work;
            work->ssylka = NULL;
        }
        work = new node;
        std::cin >> work->info;
        work->ssylka = NULL;
    }
    delete work;

    curr = head;
    while(curr != NULL) {
        std::cout << curr->info << " ";
        curr = curr->ssylka;
    }
}
/*
Доделать эти задачи
*/
