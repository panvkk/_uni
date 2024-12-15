#include <iostream>
#include <cstring>
#include <string>

int main() {
    std::string str1 = "c";
    std::string str2 = "d";
    int res = str1.compare(str2);
    std::cout << res;
}