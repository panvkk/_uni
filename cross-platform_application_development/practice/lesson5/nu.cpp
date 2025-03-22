#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <cctype>
#include <unordered_map>

using namespace std;

// Класс стека для хранения символов
class Stack {
private:
    vector<char> items;
public:
    void push(char item) {
        items.push_back(item);
    }
    char pop() {
        if (!isEmpty()) {
            char top = items.back();
            items.pop_back();
            return top;
        }
        return '\0'; // Пустой стек
    }
    char peek() const {
        if (!isEmpty()) {
            return items.back();
        }
        return '\0'; // Пустой стек
    }
    bool isEmpty() const {
        return items.empty();
    }
};

// Проверка, является ли токен операндом (однобуквенная переменная)
bool is_operand(const string& token) {
    return token.length() == 1 && isalpha(token[0]);
}

// Проверка, является ли токен оператором
bool is_operator(const string& token) {
    return token == "+" || token == "-" || token == "*" || token == "/" || token == "^";
}

// Получение приоритета оператора
int get_priority(const string& op) {
    if (op == "+" || op == "-") return 1;
    if (op == "*" || op == "/") return 2;
    if (op == "^") return 3;
    return 0;
}

// Проверка правильности расстановки скобок
bool check_brackets(const string& expression) {
    Stack stack;
    unordered_map<char, char> brackets = {{')', '('}, {']', '['}, {'}', '{'}};
    for (char ch : expression) {
        if (ch == '(' || ch == '[' || ch == '{') {
            stack.push(ch);
        } else if (ch == ')' || ch == ']' || ch == '}') {
            if (stack.isEmpty()) return false;
            char top = stack.pop();
            if (top != brackets[ch]) return false;
        }
    }
    return stack.isEmpty();
}

// Преобразование инфиксного выражения в ОПЗ с проверкой синтаксиса
pair<vector<string>, string> infix_to_postfix(const vector<string>& tokens) {
    Stack stack;
    vector<string> output;
    bool expect_operand = true; // Ожидаем операнд в начале
    unordered_map<string, string> brackets = {{")", "("}, {"]", "["}, {"}", "{"}};

    for (const string& token : tokens) {
        if (is_operand(token)) {
            if (!expect_operand) {
                return {{}, "Ожидался оператор перед " + token};
            }
            output.push_back(token);
            expect_operand = false;
        } else if (token == "(" || token == "[" || token == "{") {
            if (!expect_operand) {
                return {{}, "Ожидался оператор перед открывающей скобкой " + token};
            }
            stack.push(token[0]);
            expect_operand = true;
        } else if (token == ")" || token == "]" || token == "}") {
            if (expect_operand) {
                return {{}, "Пустые скобки у " + token};
            }
            while (!stack.isEmpty() && string(1, stack.peek()) != brackets[token]) {
                output.push_back(string(1, stack.pop()));
            }
            if (stack.isEmpty() || string(1, stack.peek()) != brackets[token]) {
                return {{}, "Непарные скобки у " + token};
            }
            stack.pop(); // Удаляем открывающую скобку
            expect_operand = false;
        } else if (is_operator(token)) {
            if (expect_operand) {
                return {{}, "Ожидался операнд перед оператором " + token};
            }
            while (!stack.isEmpty() && is_operator(string(1, stack.peek())) &&
                   get_priority(string(1, stack.peek())) >= get_priority(token)) {
                output.push_back(string(1, stack.pop()));
            }
            stack.push(token[0]);
            expect_operand = true;
        } else {
            return {{}, "Недопустимый токен: " + token};
        }
    }

    if (expect_operand) {
        return {{}, "Выражение не может заканчиваться на оператор"};
    }

    while (!stack.isEmpty()) {
        if (stack.peek() == '(' || stack.peek() == '[' || stack.peek() == '{') {
            return {{}, "Непарные скобки в конце"};
        }
        output.push_back(string(1, stack.pop()));
    }

    return {output, ""};
}

// Главная функция
int main() {
    string operators_and_brakckets = "+-*/[{()}]";
    ifstream file("text.txt");
    if (!file.is_open()) {
        cerr << "Не удалось открыть файл." << endl;
        return 1;
    }

    string expression;
    getline(file, expression);
    file.close();

    // Разбиваем строку на токены по пробелам
    vector<string> tokens;
    string token;
    for (char ch : expression) {
        if(operators_and_brakckets.find(ch) != string::npos)
            token.push_back(ch);
        else {
            if(!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
            
        }
    }
    if (!token.empty()) {
        tokens.push_back(token);
    }

    // Проверка скобок
    if (!check_brackets(expression)) {
        cout << "Ошибка: Неправильная расстановка скобок" << endl;
    } else {
        // Преобразование в ОПЗ и проверка синтаксиса
        auto [postfix, error] = infix_to_postfix(tokens);
        if (!error.empty()) {
            cout << "Ошибка: " << error << endl;
        } else {
            cout << "Выражение корректно. ОПЗ: ";
            for (const string& t : postfix) {
                cout << t << " ";
            }
            cout << endl;
        }
    }

    return 0;
}