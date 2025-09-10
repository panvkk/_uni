#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <cctype>
#include <cmath>
#include <stdexcept>

//==============================================
//  MyList, MyStack и MyQueue
//==============================================
template<typename T>
class MyList {
protected:
    struct node {
        T data;
        node* next;
    };
    node* head;
public:
    MyList() : head(nullptr) { }
    
    bool isEmpty() const { return head == nullptr; }
    
    void push_head(T data) {
        node* temp = new node;
        temp->data = data;
        temp->next = head;
        head = temp;
    }
    
    void push_tail(T data) {
        node* temp = new node;
        temp->data = data;
        temp->next = nullptr;
        if (isEmpty())
            head = temp;
        else {
            node* cur = head;
            while (cur->next)
                cur = cur->next;
            cur->next = temp;
        }
    }
    
    void show() const {
        if (isEmpty())
            std::cout << "<Пусто>" << std::endl;
        else {
            node* cur = head;
            while (cur) {
                std::cout << cur->data << " ";
                cur = cur->next;
            }
            std::cout << std::endl;
        }
    }
    
    ~MyList() {
        while (head) {
            node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

template<typename T>
class MyStack : public MyList<T> {
public:
    void push(T data) {
        this->push_head(data);
    }
    T top() const {
        if (this->isEmpty())
            throw std::runtime_error("Стек пуст!");
        return this->head->data;
    }
    T pop() {
        if (this->isEmpty())
            throw std::runtime_error("Стек пуст!");
        typename MyList<T>::node* temp = this->head;
        T data = temp->data;
        this->head = this->head->next;
        delete temp;
        return data;
    }
};

template<typename T>
class MyQueue : public MyList<T> {
public:
    void push(T data) {
        this->push_tail(data);
    }
    T front() const {
        if (this->isEmpty())
            throw std::runtime_error("Очередь пуста!");
        return this->head->data;
    }
    T pop() {
        if (this->isEmpty())
            throw std::runtime_error("Очередь пуста!");
        typename MyList<T>::node* temp = this->head;
        T data = temp->data;
        this->head = this->head->next;
        delete temp;
        return data;
    }
};

//==============================================
//  Функции
//==============================================

bool isOpeningBracket(char ch) {    // Проверка символа на открывающую скобку  
    return ch == '(' || ch == '[' || ch == '{';
}

bool isClosingBracket(char ch) {    // Проверка символа на закрывающую скобку
    return ch == ')' || ch == ']' || ch == '}';
}

bool matches(char open, char close) {   // Проверка двух скобок на соответствие открывающей и закрывающей
    return (open == '(' && close == ')') ||
           (open == '[' && close == ']') ||
           (open == '{' && close == '}');
}

bool checkBracketsBalanced(const std::string& expr) {   // Обычная проверка соответствия скобок в выражении
    MyStack<char> stack;
    for (char ch : expr) {
        if (isOpeningBracket(ch))
            stack.push(ch);
        else if (isClosingBracket(ch)) {
            if (stack.isEmpty() || !matches(stack.top(), ch))
                return false;
            stack.pop();
        }
    }
    return stack.isEmpty();
}

int getPrecedence(char op) {    // Функция для получения приоритета операции
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;
}

bool isOperator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '^';
}

// Функция для следующей
char prevNonSpace(const std::string& str, size_t idx) {
    if (idx == 0)
        return '\0';
    for (size_t i = idx; i > 0; --i) {
        if (!std::isspace(str[i - 1]))
            return str[i - 1];
    }
    return '\0';
}

std::string defaultToPolish(const std::string& infix) { // Функция для преобразования выражения в вид ОПЗ
    std::string output;
    MyStack<char> opStack;

    size_t i = 0;
    while(i < infix.size()) {
        char ch = infix[i];
        if (ch == ' ') {
            i++;
            continue;
        }
        if (isalnum(ch)) {  // Читаем целиком слово (число или переменная)
            while(i < infix.size() && isalnum(infix[i])) {
                output.push_back(infix[i]);
                i++;
            }
            output.push_back(' ');
        } else if (isOpeningBracket(ch)) {
            opStack.push(ch);
            output.push_back(' ');
            i++;
        } else if (isClosingBracket(ch)) {
            output.push_back(' ');
            while(!opStack.isEmpty() && !isOpeningBracket(opStack.top())) {
                output.push_back(opStack.pop());
                output.push_back(' ');
            }
            if (!opStack.isEmpty() && isOpeningBracket(opStack.top()))
                opStack.pop();
            i++;
        } else if (isOperator(ch)) {
            // Проверка, является ли оператор унарным
            char prev = prevNonSpace(infix, i);
            if ((ch == '-' || ch == '+') && (prev == '\0' || isOpeningBracket(prev) || isOperator(prev))) {
                if (ch == '-') {
                    output.append("0 ");
                }
            }
            output.push_back(' ');
            while(!opStack.isEmpty() && isOperator(opStack.top()) &&
                  getPrecedence(ch) <= getPrecedence(opStack.top())) {
                output.push_back(opStack.pop());
                output.push_back(' ');
            }
            opStack.push(ch);
            i++;
        } else {
            // Пропуск неизвестных символов
            i++;
        }
    }
    while(!opStack.isEmpty()){
        output.push_back(' ');
        output.push_back(opStack.pop());
    }
    return output;
}



double evaluatePostfix(const std::string& postfix, const std::map<std::string, double>& variables) {
    MyStack<double> stack;
    std::istringstream iss(postfix);
    std::string token;
    while(iss >> token) {
        if(token.size() == 1 && isOperator(token[0])) {
            double b = stack.pop();
            double a = stack.pop();
            char op = token[0];
            double res = 0.0;
            switch(op) {
                case '+': res = a + b; break;
                case '-': res = a - b; break;
                case '*': res = a * b; break;
                case '/': res = a / b; break;
                case '^': res = pow(a, b); break;
                default: break;
            }
            stack.push(res);
        } else {
            try {
                double val = std::stod(token);
                stack.push(val);
            } catch(...) {
                if (variables.find(token) != variables.end())
                    stack.push(variables.at(token));
                else
                    stack.push(1.0);
            }
        }
    }
    return stack.pop();
}

//==============================================
//  Main
//==============================================
int main() {
    setlocale(LC_ALL, "ru_RU.UTF-8");

    std::ifstream infile("input.txt");
    if(!infile) {
        std::cerr << "Не удалось открыть файл input.txt" << std::endl;
        system("pause"); return 1; 
    }

    std::cout << "Здравствуйте, дедушка. Данная программа читает из файла input.txt данные:\n"
              << "1 строка - алгебраическое выражение\n2 строка - значения для переменных в этом выражении.\n"
              << "Кроме того, если найдена ошибка, программа предложит варианты для исправления.\n\n";
    
    // Читаем первую строку – алгебраическое выражение.
    std::string infix;
    std::getline(infile, infix);
    std::cout << "Введённое выражение: " << infix << std::endl;
    
    // Дополнительные проверки записи выражения:
    if(!checkBracketsBalanced(infix)) {
        std::cout << "Ошибка: баланс скобок нарушен! Не дрожат ли у дедушки ручонки?" << std::endl;
        system("pause"); return 1; 
    }
    if(infix.find("}[") != std::string::npos ||
        infix.find("}(") != std::string::npos ||
        infix.find("){") != std::string::npos ||
        infix.find(")[") != std::string::npos ||
        infix.find("](") != std::string::npos ||
        infix.find("]{") != std::string::npos ) {
        std::cout << "Дедушка, возможно, вы забыли знак операции между скобками. Проверьте выражение." << std::endl;
        system("pause"); return 1; 
    }
    if(infix.find("[]") != std::string::npos ||
       infix.find("{}") != std::string::npos ||
       infix.find("()") != std::string::npos) {
        std::cout << "Обнаружены пустые скобки. Дедушка, возможно, вы пропустили значение или операцию внутри скобок." << std::endl;
        system("pause"); return 1; 
    }
    
    // Преобразуем инфиксную запись в ОПЗ.
    std::string postfix = defaultToPolish(infix);
    std::cout << "Обратная польская запись: " << postfix << std::endl;
    
    std::string varsLine;
    std::getline(infile, varsLine);
    std::map<std::string, double> variables;
    std::istringstream iss(varsLine);
    std::string token;
    while(iss >> token) {
        size_t pos = token.find('=');
        if(pos != std::string::npos) {
            std::string var = token.substr(0, pos);
            double val = std::stod(token.substr(pos+1));
            variables[var] = val;
        }
    }
    infile.close();
    
    // Если выражение правильное – вычисляем его.
    try {
        double result = evaluatePostfix(postfix, variables);
        std::cout << "Результат выражения: " << result << std::endl;
    }
    catch(const std::exception& ex) {
        std::cerr << "Ошибка при вычислении выражения: " << ex.what() << std::endl;
        system("pause"); return 1; 
    }

    system("pause");
    return 0;
}