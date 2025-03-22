#include <iostream>
#include <fstream>
#include <cstdio>
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
    MyList() : head(nullptr) { }

    bool isEmpty() {
        return head == nullptr;
    }

    node* getHead() {
        return head;
    }

    void push_head(T data) {
        node* temp = new node;
        temp->data = data;
        temp->next = head;
        head = temp;
    }
    
    void push_tail(T data) {
        if (isEmpty()) { 
            head = new node; 
            head->data = data; 
            head->next = nullptr; 
        } else { 
            node* temp = head; 
            while (temp->next) { 
                temp = temp->next; 
            }
            temp->next = new node;
            temp->next->data = data;
            temp->next->next = nullptr;
        }
    } 
    
    void show() {
        node* temp = head;
        if (isEmpty()) {
            std::cout << "out: <пусто>" << std::endl;
        } else { 
            while (temp) { 
                std::cout << "Адрес элемента - " << temp 
                          << ", Информационная часть - " << temp->data 
                          << ", Адрес следующего элемента - " << temp->next << std::endl;
                temp = temp->next; 
            }
        }
    }
};

template <typename T>
class MyStack : public MyList<T> {
public:
    void push(T data) {
        this->push_head(data);
    }
    T pop() {
        if(this->isEmpty()) {
            return T{};
        }
        auto temp = this->getHead(); 
        T data = temp->data;
        this->head = temp->next;
        delete temp;
        return data;
    }
    T top() {
        if(!this->isEmpty())
            return this->head->data;
        return T{};
    }
};

std::map<char, int> operand_hierarhcy {
    { '-', 1 },
    { '+', 1 },
    { '*', 2 },
    { '/', 2 }
};

int main() {
    setlocale(LC_ALL, "Russian");

    std::cout << "Здравствуйте, дедушка. Эта программа проверяет выражение из файла text.txt на правильность,\n"
              << "переводя в вид обратной польской записи. Введите пример в файл и проверьте его на правильность.\n\n";

    std::string operators_and_brakckets = "+-*/[{()}]";
    std::string result;
    std::string error;
    std::string causer_of_error;

    char ch;
    MyStack<char> stack;
    std::string token;
    bool indicator = true;
    bool expect_token = true;    

    std::ifstream file("text.txt");
    if (!file) {
        std::cout << "Ошибка: не удалось открыть файл text.txt. \nВозможно, он находится не в нужной директории." << std::endl;
        system("pause");
    }
    
    while(file.get(ch) && indicator) {
        while(operators_and_brakckets.find(ch) == std::string::npos && file) {
            token.push_back(ch);
            file.get(ch);
        }
        if(!token.empty()) {
            if(!expect_token) {
                indicator = false;
                error = "Ожидался оператор.\n";
                causer_of_error = token;
            }
            result.append(token);
            result.push_back(' ');
            token.clear();
            expect_token = false;
        }
        
        if(ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        } 
        else if(ch == '*' || ch == '-' || ch == '+' || ch == '/') {
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

        } 
        else if(ch == ')' || ch == ']' || ch == '}') {
            
            if(stack.isEmpty()) {
                indicator = false;
                error = "Ожидался операнд, а получена закрывающая скобка.\n";
                causer_of_error = ch;
            }
            char operand = stack.pop();
            if(ch == ')') {
                while(operand != '(') {
                    if(operand == '{' || operand == '[') {
                        indicator = false;
                        error = "Ожидалась скобка ) , но встречана другая открывающая.\n";
                        causer_of_error = operand;
                    }
                    result.push_back(operand);
                    result.push_back(' ');
                    if(stack.isEmpty()) {
                        indicator = false;
                        error = "Отсутствует открывающая скобка для ')'.\n";
                    }
                    operand = stack.pop();
                }
            }
            else if(ch == '}') {
                while(operand != '{') {
                    if(operand == '(' || operand == '[') {
                        indicator = false;
                        error = "Ожидалась скобка } , но встречана другая открывающая.\n";
                        causer_of_error = operand;
                    }
                    result.push_back(operand);
                    result.push_back(' ');
                    if(stack.isEmpty()) {
                        indicator = false;
                        error = "Отсутствует открывающая скобка для '}'.\n";
                    }
                    operand = stack.pop();
                }
            }
            else if(ch == ']') {
                while(operand != '[') {
                    if(operand == '(' || operand == '{') {
                        indicator = false;
                        error = "Ожидалась скобка ] , но встречана другая открывающая.\n";
                        causer_of_error = operand;
                    }
                    result.push_back(operand);
                    result.push_back(' ');
                    if(stack.isEmpty()) {
                        indicator = false;
                        error = "Отсутствует открывающая скобка для ']'.\n";
                    }
                    operand = stack.pop();
                }
            }
        } 
    }

    while(!stack.isEmpty()) {
        ch = stack.pop();
        result.push_back(ch);
        result.push_back(' ');
    }
    
    if(indicator)
        std::cout << "Пример верный!\n" << "Постфиксная запись:\n" << result << std::endl;
    else {
        std::cout << "Ошибка: " << error 
                  << "Виновник ошибки: " << causer_of_error << std::endl;
    }

    system("pause");
    return 0;
}
