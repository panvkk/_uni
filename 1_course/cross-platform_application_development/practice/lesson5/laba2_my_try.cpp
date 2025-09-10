#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string>
#include <map>

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
        T pop() {
            T data;
            if(this->isEmpty()) return T{};
            else {
                auto temp = this->getHead(); 
                data = temp->data;
                temp = temp->next;
                delete this->head;
                this->head = temp;
            }
            return data;
        }
        T top() {
            if(!this->isEmpty())
                return this->head->data;
            return 0;
        }
};

std::map<char, int> operand_hierarhcy {
    { '-', 1 },
    {'+' , 1},
    {'*', 2},
    {'/', 2}
};


int main() {
    setlocale(LC_ALL, "Russian");
    std::string operators_and_brakckets = "+-*/[{()}]";
    std::string result;
    std::string error, causer_of_error;


    char ch;
    MyStack<char> stack;
    std::string token;
    bool indicator = true;
    bool expect_token = true;
    bool expect_operator_or_token;
    
    std::ifstream file("text.txt");
    
    while(file.good() && indicator) {
        file.get(ch);
        while(operators_and_brakckets.find(ch) == std::string::npos && file.good()) {
            token.push_back(ch);
            file.get(ch);
        }
        if(!token.empty()) {
            if(!expect_token) {
                indicator = false;
                error = "Ожидался оператор.\n";
                causer_of_error = token;
            }
            for(char i : token)
                result.push_back(i);
            result.push_back(' ');
            token.clear();
            expect_token = false;
        } 
        if(ch == '(' || ch == '[' || ch == '{') {
            expect_operator_or_token = true;
            stack.push(ch);
        } else if(ch == '*' || ch == '-' || ch == '+' || ch == '/') {
            while(!stack.isEmpty() &&
                stack.top() != '(' &&
                stack.top() != '[' &&
                stack.top() != '{' &&
                operand_hierarhcy[stack.top()] >= operand_hierarhcy[ch]
            ) {
                result.push_back(stack.top());
                result.push_back(' ');
                stack.pop();
            }
            stack.push(ch);
            expect_token = true;
        } else if(!stack.isEmpty() && (ch == ')' || ch == ']' || ch == '}')) {

            if(expect_token || expect_operator_or_token) {
                indicator = false;
                error = "Ожидался операнд, а не закрываяющая скобка.\n";
                causer_of_error = ch;
            }
            char operand = stack.pop();
            if(ch == ')')
                while(operand != '(') {
                    if(operand == '{' || operand == '[') {
                        indicator = false;
                        error = "Нарушен порядок и соответсвие скобок.\n";
                        causer_of_error = operand;
                    }
                    result.push_back(operand);
                    result.push_back(' ');
                    operand = stack.pop();
                }
            else if(ch == '}')
                while(operand != '{') {
                    if(operand == '(' || operand == '[') {
                        indicator = false;
                        error = "Нарушен порядок и соответсвие скобок.\n";
                        causer_of_error = operand;
                    }
                    result.push_back(operand);
                    result.push_back(' ');
                    operand = stack.pop();
                }
            else if(ch == ']')
                while(operand != '[') {
                    if(operand == '(' || operand == '{') {
                        indicator = false;
                        error = "Нарушен порядок и соответсвие скобок.\n";
                        causer_of_error = operand;
                    }
                    result.push_back(operand);
                    result.push_back(' ');
                    operand = stack.pop();
                }
        } else if(result.empty() && ch == ')' || ch == ']' || ch == '}') {
            indicator = false;
            error = "Закрывающая скобка без открывающей.\n";
            causer_of_error = ch;
        }
    }

    while(!stack.isEmpty()) {
        ch = stack.pop();
        result.push_back(ch);
        result.push_back(' ');
    }
    
    if(indicator)
        std::cout << result;
    else {
        std::cout << "Ошибка: " << error << "Виновник ошибки: " << causer_of_error << std::endl;
    }

    system("pause");
}