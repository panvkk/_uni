#include <stdio.h>
#include <iostream>
#include <string>

#define MAX_LEN 31

void getInput(std::ifstream, std::string&);

struct Student {
    char fullname[MAX_LEN];
    short int year;                    
    unsigned char month;                  
    unsigned char day;                   
    unsigned short int admissionScore;        
};


int main() {
    struct Student student;
    char char_name[MAX_LEN];
    std::cout << "Введите данные через пробел(ФИО, год рождения, месяц рождения, день рождения,\nбалл при поступлении): ";
    std::string fullname;
    getInput(std::cin, fullname);

    for(int i = 0; i < 30; i++)
        char_name[i] = fullname[i];
    

}

void getInput(std::istream& cout, std::string& input) {
    char ch;
    char token[MAX_LEN];
    for(int i = 0; std::cin.get(ch); i++) {
        if(std::isalnum(ch)) {
            token[i] = ch;
        } else if(ch == ' ') {
            token[i] = '\0';
            input = token;
            break;
        } else {
            i--;
        }
    }
}