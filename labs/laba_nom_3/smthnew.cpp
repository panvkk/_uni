#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include "my_functions.h"

int main()
{
    srand(time(0));
    createFile();
    std::ifstream FILE("Input.txt");
    std::ofstream oFILE("Input.txt");
    std::string str;
    std::getline(FILE, str);
    oFILE << str << "\n";
    while (!FILE.eof()) { 
        std::string str1;
        std::getline(FILE, str1);
    }
    return 0;
}