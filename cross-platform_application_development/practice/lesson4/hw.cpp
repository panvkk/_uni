
#include <iostream>

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
            if(isEmpty()) {
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
        void showList() {
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

int main() {
    setlocale(LC_ALL, "Russian");
    MyList<char> List;
    std::cout << "Введите символы с клавиатуры. Для прекращения ввода следует нажать клавишу Enter:\n"; 

    char ch;
    while(std::cin.get(ch) && ch != '\n') {
        List.push_tail(ch);
    }
    List.showList();
    system("pause");
}