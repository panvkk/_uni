#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>
#include <queue>
#include <fstream>

struct info { 
    std::string book_name;
    int year;
    int capacity;

    info(std::string a, int b, int c) : book_name(a) , year(b), capacity(c)
        {}
};

class Tbook {
        int lines_quantity;
        std::priority_queue<std::string> autors;
        std::multimap<std::string, info> data;
    public: 
        Tbook() : lines_quantity(0)
            {}
        Tbook(int a, std::priority_queue<std::string> b, std::multimap<std::string, info> c) : lines_quantity(a) , autors(b) , data(c)
            {}
        void add_autor(std::string& n) {
            autors.push(n);
        }
        void add_data(std::string n, info c_i) {
            data.insert({n, {c_i}});
        }
        
        

};

int main() {
    std::ifstream books("Book.txt");
    Tbook all_autors;
    while(books.good()) {
        std::string curr_line, name_line, book_line, date, pages;
        getline(books, curr_line);

        //############################

        name_line = curr_line.substr(0, curr_line.find_first_of(':'));
        book_line = curr_line.substr(curr_line.find_first_of(":"), curr_line.find_first_of(";"));
        date = curr_line.substr(curr_line.find_first_of(";"), curr_line.find_last_of("-"));
        date = std::stoi(date, nullptr   date.find_first_of("1234567890"));

        pages = curr_line.substr(curr_line.find_last_of("-"), std::string::npos - curr_line.find_last_of("-"));
        info current_info();
        //############################
        std::string name;
        for( auto index = name_line.begin(); index != name_line.end(); index++) {
            if(*index == ',' || *index == '\n') {
                if(!name.empty()) {
                    std::string word;
                    for(auto i = name.begin(); i != name.end(); i++) {
                        if(*i == ' ') {
                            if(!word.empty() || word.find('.') == std::string::npos) {
                                all_autors.add_autor(word);
                                all_autors.data[word] = 
                            }
                        }
                    }
                }
            } else {
                name.push_back(*index);
            }
        }
        // if(name_line.find(".") != std::string::npos) {
        //     std::string name;
        //     for(auto iter = name_line.begin(); iter != name_line.end(); iter++) {
        //         if(*iter != ' ' || *iter != ',' || *iter != '\n')
        //             name.push_back(*iter);
        //         else if(!name.empty()) { 
        //             if(name.find(".") == std::string::npos) {
        //                 all_autors.add_autor(name);
        //                 break;
        //             } else {
        //                 name.clear();
        //             }
        //         }
        //     }
        //     if(name.empty()) {
        //         std::cout << "Ошибка с именем!\n"; continue; 
        //     }
        // } else {

        // }
    }
}



