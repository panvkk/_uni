#include <iostream>
#include <fstream>
#include <sstream>
#include <regex>
#include <string>
#include <list>

using namespace std;

class MyList {
public:
    virtual void push(char c) = 0;
    virtual char pop() = 0;
    virtual bool empty() const = 0;
    virtual ~MyList() {}
};

class MyStack : public MyList {
private:
    list<char> container;
public:
    virtual void push(char c) override {
        container.push_back(c);
    }
    
    virtual char pop() override {
        char c = container.back();
        container.pop_back();
        return c;
    }
    
    virtual bool empty() const override {
        return container.empty();
    }
};

class MyQueue : public MyList {
private:
    list<char> container;
public:
    virtual void push(char c) override {
        container.push_back(c);
    }
    
    virtual char pop() override {
        char c = container.front();
        container.pop_front();
        return c;
    }
    
    virtual bool empty() const override {
        return container.empty();
    }
};

string reverseNumber(const string &numStr) {
    MyStack stack;
    for (char c : numStr) {
        stack.push(c);
    }
    string reversed;
    while (!stack.empty()) {
        reversed.push_back(stack.pop());
    }
    return reversed;
}

int main() {
    string filename = "mrak.txt";
    ifstream infile(filename);
    if (!infile) {
        cerr << "Ошибка открытия файла: " << filename << endl;
        return 1;
    }
    
    stringstream buffer;
    buffer << infile.rdbuf();
    string text = buffer.str();
    infile.close();
    
    regex re("\\d+");
    string result;
    
    size_t last_pos = 0;
    auto matches_begin = sregex_iterator(text.begin(), text.end(), re);
    auto matches_end = sregex_iterator();
    for (auto it = matches_begin; it != matches_end; ++it) {
        smatch match = *it;
        size_t pos = match.position();
        result.append(text, last_pos, pos - last_pos);
        
        string numStr = match.str(); 
        string reversedNum = reverseNumber(numStr);
        result.append(reversedNum);
        
        last_pos = pos + match.length();
    }
    result.append(text, last_pos, text.size() - last_pos);
    
    ofstream outfile(filename);
    if (!outfile) {
        cerr << "Ошибка записи в файл: " << filename << endl;
        return 1;
    }
    outfile << result;
    outfile.close();
    
    return 0;
}
