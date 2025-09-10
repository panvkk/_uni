#include <iostream>
#include <fstream>
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
        node* temp = new node;
        temp->data = data;
        temp->next = head;
        head = temp;
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
};

template <typename T>
class MyStack : public MyList<T> {
public:
    void push(T data) {
        this->push_head(data);
    }
    T pop() {
        if (this->isEmpty()) return T{};
        T data = this->head->data;
        auto temp = this->head;
        this->head = this->head->next;
        delete temp;
        return data;
    }
    T top() {
        if (!this->isEmpty())
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
    std::string operators_and_brackets = "+-*/[{()}]";
    std::string result;
    std::string error;
    char causer_of_error = '\0';

    MyStack<char> stack;
    std::string token;
    bool indicator = true;
    bool expect_token = true;
    bool expect_operator_or_token = false;

    std::ifstream file("text.txt");
    if (!file.is_open()) {
        std::cout << "Ошибка: Не удалось открыть файл text.txt\n";
        system("pause");
        return 1;
    }

    char ch;
    while (file.get(ch) && indicator) {
        // Собираем токен (операнд), пока не встретим оператор или скобку
        while (operators_and_brackets.find(ch) == std::string::npos && !file.eof()) {
            token.push_back(ch);
            if (!file.get(ch)) break; // Проверяем, не конец ли файла
        }
        if (!token.empty()) {
            if (!expect_token) {
                indicator = false;
                error = "Ожидался оператор.";
                causer_of_error = token[0];
            } else {
                result += token + " ";
                expect_token = false;
            }
            token.clear();
        }

        // Обработка символа (оператор или скобка)
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
            expect_token = true;
            expect_operator_or_token = true;
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.isEmpty()) {
                indicator = false;
                error = "Закрывающая скобка без открывающей.";
                causer_of_error = ch;
            } else if (expect_token || expect_operator_or_token) {
                indicator = false;
                error = "Ожидался операнд, а не закрывающая скобка.";
                causer_of_error = ch;
            } else {
                char opening = (ch == ')') ? '(' : (ch == '}') ? '{' : '[';
                while (!stack.isEmpty() && stack.top() != opening) {
                    char top = stack.top();
                    if (top == '(' || top == '{' || top == '[') {
                        indicator = false;
                        error = "Нарушен порядок и соответствие скобок.";
                        causer_of_error = top;
                        break;
                    }
                    result.push_back(stack.pop());
                    result.push_back(' ');
                }
                if (!stack.isEmpty() && stack.top() == opening) {
                    stack.pop(); // Удаляем открывающую скобку
                    expect_token = false;
                    expect_operator_or_token = false;
                } else {
                    indicator = false;
                    error = "Не найдена соответствующая открывающая скобка.";
                    causer_of_error = ch;
                }
            }
        } else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            while (!stack.isEmpty() && stack.top() != '(' && stack.top() != '[' && stack.top() != '{' &&
                   operand_hierarhcy.find(stack.top()) != operand_hierarhcy.end() &&
                   operand_hierarhcy[stack.top()] >= operand_hierarhcy[ch]) {
                result.push_back(stack.pop());
                result.push_back(' ');
            }
            stack.push(ch);
            expect_token = true;
            expect_operator_or_token = false;
        }
    }

    // Обработка оставшегося токена, если файл закончился
    if (!token.empty() && indicator) {
        if (!expect_token) {
            indicator = false;
            error = "Ожидался оператор.";
            causer_of_error = token[0];
        } else {
            result += token + " ";
        }
    }

    // Выгрузка оставшихся операторов из стека
    while (!stack.isEmpty() && indicator) {
        char top = stack.pop();
        if (top == '(' || top == '[' || top == '{') {
            indicator = false;
            error = "Не закрыта открывающая скобка.";
            causer_of_error = top;
        } else {
            result.push_back(top);
            result.push_back(' ');
        }
    }

    // Вывод результата
    if (indicator) {
        std::cout << "Результат: " << result << std::endl;
    } else {
        std::cout << "Ошибка: " << error << " Виновник ошибки: " << causer_of_error << std::endl;
    }

    file.close();
    system("pause");
    return 0;
}