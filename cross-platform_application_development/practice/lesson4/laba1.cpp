
#include <iostream>


/* ---------------------------------------------------------------
                            Container
-----------------------------------------------------------------*/
template<typename T>
class Container {
    protected:
        struct node {
            T data;
            node* next;
        };
        node* head;
    public:
        Container() : head(NULL) { }

        bool isEmpty() {
            return head ? false : true;
        }

        node* getHead() {
            return head;
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
        node* findByValue(T data){
            auto temp = getHead();
            while (temp != NULL) { 
                if (temp->data == data) 
                    return temp; 
                else 
                    temp = temp->next; 
            }
            return NULL; 
        }
        void deleteElement(T data) {
            node* temp = findByValue(data);
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
};

/* ---------------------------------------------------------------
                            MyList
-----------------------------------------------------------------*/

template<typename T>
class MyList : public Container<T> {
    public: 
        void push_head(T data) {
            auto temp = new class Container<T>::node;
            temp->data = data;
            if(this->isEmpty()) {
                temp->next = NULL;
                this->head = temp;
            } else {
                temp->next = this->head;
                this->head = temp;
            }
        }
        void push_tail(T data) {
            class Container<T>::node* temp;
            if(this->isEmpty()) { 
                this->head = new class Container<T>::node; 
                this->head->next = NULL; 
                this->head->data = data; 
            } else { 
                temp = this->head; 
                while (temp->next) { 
                    temp = temp->next; 
                }
                temp->next = new class Container<T>::node;
                temp->next->next = NULL;
                temp->next->data = data; 
            }
        } 
        T getBack() {
            if (this->isEmpty()) 
                throw "Список пуст!\n";
            else {
                auto temp = this->head;
                while (temp->next) 
                    temp = temp->next;
                return temp->data;
            }
        }
        T getFront() {
            if (this->isEmpty()) 
                throw "Список пуст!\n";
            else 
                return this->head->data;
        }
};

/* ---------------------------------------------------------------
                            MyStack
-----------------------------------------------------------------*/

template <typename T>
class MyStack : public Container<T> {
    public:
        T top() {
            if(this->isEmpty) 
                throw "Стэк пуст!\n";
            else 
                return this->head->data;
        }
        void push(T value) {
            if (this->is_empty()) {
                this->head = new class Container<T>::node;
                this->head->data = value;
                this->head->next = NULL;
            }
            else {
                auto temp = new class Container<T>::node;
                temp->data = value;
                temp->next = this->head;
                this->head = temp;
            }
        }
        T pop() {
            if (this->isEmpty()) throw "Стэк пуст!\n";
            else {
                T value = this->head->data;
                auto temp = this->head->next;
                delete this->head;
                this->head = temp;
                return value;
            }
        };
};

/* ---------------------------------------------------------------
                            MyQueue
-----------------------------------------------------------------*/

template <typename T>
class MyQueue : public Container<T> {
    public:
        void push(T value) {
            if(this->is_empty()) {
                this->head = new class Container<T>::Node;
                this->head->data = value;
                this->head->next = NULL;
            }
            else {
                auto temp = this->head;
                while(temp->next) 
                    temp = temp->next;
                temp->next = new class Container<T>::Node;
                temp->next->data = value;
                temp->next->next = NULL;
            }
        }
        T front() {
            if (this->is_empty()) 
                throw "Очередь пуста!\n";
            else 
                return this->head->data;
        }
        T back() {
            if (this->is_empty()) 
                throw "Очередь пуста!\n";
            else {
                auto curr = this->head;
                while(curr->next) 
                    curr = curr->next;
                return curr->data;
            }
        }
        T pop() {
            if (this->is_empty()) 
                throw "Очередь пуста!\n";
            else {
                T value = this->head->data;
                auto temp = this->head->next;
                delete this->head;
                this->head = temp;
                return value;
            }
        }
};

/* ---------------------------------------------------------------
                            Task
-----------------------------------------------------------------*/

int main() {
    setlocale(LC_ALL, "Russian");
    MyList<int> List;
    auto head = List.getHead();
    int value;
    std::cout << "Введите через пробел последовательность чисел, оканчивающуюся нулём: \n";
    std::cin >> value;
    while(value) {
        List.push_head(value);
        auto value_ptr = List.findByValue(value);
        while(value_ptr->next != NULL && value_ptr->data > value_ptr->next->data) {
            std::swap(value_ptr->data, value_ptr->next->data);
            value_ptr = value_ptr->next;
        }
        std::cin >> value;
    }
    List.show();
    system("pause");
}
