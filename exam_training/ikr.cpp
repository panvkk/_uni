#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <queue>
#include <fstream>

class Tbook {
        int line_number;
        std::priority_queue<std::string> autors;
        std::multimap<std::string, std::string> all_info;
    public: 
        

};

int main() {
    std::ifstream books("Book.txt");
    Tbook info;
    while(books.good()) {
        std::string current_line, current_info;
        getline(books, current_line);
        int index = 0;
        char ch;
        for(index; ch != ':'; index++) {
            current_info.push_back(ch);
            ch = current_line[index];
        }
    }
}