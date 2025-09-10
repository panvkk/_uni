#include <iostream>
#include <fstream>
#include <string>
#include <vector>

int func1(int n, int m = 3)
{
    return n+m;
}
int func2(int a, int b = 0, char c = ' ')
{
    std::cout << "sdfs" << c << "sdfd\n";
    return a+b;
}
int main()
{
    std::ifstream FILE("Input.txt");
    std::string str;
    std::getline(FILE, str);
    std::vector<std::string> vector;
    while (!str.empty()) { 
        std::string word;
        size_t pos = str.find_first_of(",. "), beg = 0;
        for (int k = 0; k < pos; k++) {
            word.push_back(str[k]);
        }
        str.erase (beg, ++pos);
        std::cout << str << "\n";
        if(!word.empty()) 
            vector.push_back(word);
    }
    for(int i = 0; i < vector.size(); i++) {
        std::cout << vector[i] << " "; }
    return 0;
}