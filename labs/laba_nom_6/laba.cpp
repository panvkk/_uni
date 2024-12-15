#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <iterator>

int main() {
    std::list<int> list, new_list;
    std::ifstream infile("Input.txt"); std::ofstream outfile("Output.txt");
    std::string number;
    char ch;
    while(infile.get(ch)) {
        if( ch == ' ') {
            list.push_back(std::stoi(number));
            number.clear();
        } else 
            number.push_back(ch);
    }
    if(number != "")
        list.push_back(std::stoi(number));

    while((list.size()/3) >= 1 ) {
        int first = list.front(); list.pop_front();
        int second = list.front(); list.pop_front();
        int third = list.front(); list.pop_front();
        third += second;
        second += first;
        new_list.push_back(first); new_list.push_back(second); new_list.push_back(third);
    }
    while(!list.empty()) {
        new_list.push_back(list.front());
        list.pop_front();
    }
    while(!new_list.empty()) {
        outfile << new_list.front() << " ";
        new_list.pop_front();
    }
    infile.close();
    outfile.close();
    return 0;
}