#include <iostream>
#include <fstream>
#include <stdio.h>

template<typename T>
class MyList {
    public:
        struct node {
            T data;
            node* next;
        };
    protected:
        node* head;
    public:

        MyList() : head(NULL) { }

        bool isEmpty() {
            return head ? false : true;
        }

        node* getHead() {
            return head;
        }

        void push_head(T data) {
            node* temp;
            temp = new node;
            temp->data = data;
            temp->next = NULL;
            if(isEmpty()) {
                temp->next = NULL;
                head = temp;
            } else {
                temp->next = head;
                head = temp;
            }
        }
        
        void push_tail(T data) {
            node* temp;
            if (isEmpty()) { 
                head = new node; 
                head->next = NULL; 
                head->data = data; 
            } else { 
                temp = head; 
                while (temp->next) { 
                    temp = temp->next; 
                }
                temp->next = new node;
                temp->next->next = NULL;
                temp->next->data = data; 
            }
        } 
        void show() {
            node* temp = head;
            if (isEmpty()) 
                std::cout << "out: <пусто>" << std::endl;
            else { 
                std::cout << "out:\n" << "Адрес элемента - " << temp << ", Информационная часть - " << temp->data << ", Адрес следующего элемента - " << temp->next << std::endl;
                temp = temp->next;
                while (temp) { 
                    std::cout << "Адрес элемента - " << temp << ", Информационная часть - " << temp->data << ", Адрес следующего элемента - " << temp->next << std::endl; 
                    temp = temp->next; 
                }
            }    
        }
        node* findValueByData(T data){
            node* temp = head;
            while (temp != NULL) { 
                if (temp->data == data) return temp; temp = temp->next; 
            }
            return NULL; 
        }
        void deleteElement(T data) {
            node* temp = findValueByData(data);
            node* save = head;
            if (!isEmpty() && temp != NULL) {
                if (temp == head) { 
                    node* temp = head; 
                    temp = temp->next; 
                    delete head; 
                    head = temp; 
                }
                else { 
                    while (save->next != temp) save = save->next; save->next = temp->next; delete temp; 
                }
            }
        }
        void deleteElement(node* value) {
            node* curr = head;
            
            if(value != head) {
                while(curr->next != value) {
                    curr = curr->next;
                }
                if(value->next == NULL) {
                    curr->next = NULL;
                    delete value;
                } else {
                    curr->next = value->next;
                    delete value;
                }
            } 
            else {
                head = head->next;
                delete value;
            }
        }
        int findMaxValue(node*& pointer_to_max) {
            int max_element;
            node* curr;
            max_element = 0;
            curr = head;
            while(curr) {
                if(curr->data > max_element) {
                    max_element = curr->data;
                    pointer_to_max = curr;
                } else {
                    curr = curr->next;
                }
            }
            return max_element;
        }
        int findMaxValue() {
            int max_element;
            node* curr;
            max_element = 0;
            curr = head;
            while(curr) {
                if(curr->data > max_element) {
                    max_element = curr->data;
                } else {
                    curr = curr->next;
                }
            }
            return max_element;
        }
        
};

template <typename T>
class MyStack : public MyList<T> {
    public:
        void push(T data) {
            this->push_head(data);
        }
        bool pop() {
            if(this->isEmpty()) return false;
            else {
                auto temp = this->getHead(); 
                temp = temp->next;
                delete this->head;
                this->head = temp;
            }
            return true;
        }
        T top() {
            if(!this->isEmpty())
                return this->head->data;
            return 0;
        }
};

int main() {
    setlocale(LC_ALL, "Russian");
    char ch;
    bool indicator;
    MyStack<char> stack;
    
    std::ifstream file("text.txt");
    
    indicator = true;
    while(file.good() && indicator) {
        file.get(ch);
        std::cout << ch;
        if(ch == '(') {
            stack.push(ch);
        } else if(ch == ')') {
            if(stack.top() != '(')
                indicator = false;
            else 
                indicator = stack.pop();
        } else if(ch == '[') {
            stack.push(ch);
        } else if(ch == ']') {
            if(stack.top() != '[')
                indicator = false;
            else
                indicator = stack.pop();
        }
        ch = ' ';
    }
    if(!indicator || !stack.isEmpty()) {
        std::cout << "\033[3;31m|<-- Тут ошибка!|\033[0m";
        while(file.good()) {
            file.get(ch); 
            std::cout << ch; 
            ch = ' ';
        }
        std::cout << "\n\033[31mЗапись ошибочна!\033[0m\n";
    } else {
        std::cout << "\n\033[32mЗапись верна!\033[0m\n";
    }
    system("pause");
}

/*
Домашнее задание: домашка из файла + каким-то образом необходимо выделить в какой момент времени или в каком месте исходного файла
обнаруживается ошибка. Можно эту скобку или её отсутствие вывести на экран другим цветом либо снизу под строчкой файла нарисовать стрелочку
вверх, что там произошла ошибка.

Следующая неделя Qt. Надо инсталировать Qt, предполагая, что для выполнения лаб работ понадобится прорисовка изображений
и звук(музыка). В последнем файле есть исполнимый модуль(чучело) - это может быть лабораторной работой 3. Есть движующееся 
изображение и музыка. Смысл этого чучела догнать фотографию Константина Антоновича и сколько-то раз используя ЛКМ убить 
фотографию Константина Антоновича. После убийства должен появиться катафалк и музыка из похорон.
В последнем письме есть информация про инсталяцию Qt и инфа о том, что можно делать с её помощью. К некст занятию 
вызубрить всё, что записано в файле Window. 
Надо будет перерисовать все компоненты окон приложения на русском и английском языках.
Можно и начинать лаб работу 2. 
Пожелание: письма и файлы, которые передают следуют читать. В последнем письме есть хог_1, хог_2
*/  