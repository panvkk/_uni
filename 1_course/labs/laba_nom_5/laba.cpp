#include <iostream>
#include <fstream>
#include <stack>
#include <string>

int main() {
    std::stack<int> stack, new_stack;
    std::ifstream infile("Input.txt"); std::ofstream outfile("Output.txt");
    std::string number;
    char ch;
    while(infile.get(ch)) {
        if( ch == ' ') {
            stack.push(std::stoi(number));
            number.clear();
        } else 
            number.push_back(ch);
    }
    if(number != "")
        stack.push(std::stoi(number));
    int max_number = 0;
    while(!stack.empty()) {
        if(max_number < stack.top())
            max_number = stack.top();
        new_stack.push(stack.top());
        stack.pop();
    }
    while(!new_stack.empty()) {
        if(new_stack.top() != max_number)
            stack.push(new_stack.top());
        new_stack.pop();
    }
    while(!stack.empty()) {
        new_stack.push(stack.top());
        stack.pop();
    }
    while(!new_stack.empty()) {
        outfile << new_stack.top() << " ";
        new_stack.pop();
    }
    infile.close();
    outfile.close();
    return 0;
}