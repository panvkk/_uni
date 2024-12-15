#include <iostream>

int main()
{
    int des,binary,mnozh=1;
    std::cout << "Введите число в двоичном виде: \n";
    std::cin >> binary;
    while (binary > 0) {
        int d = binary % 10;
        des += d * mnozh;
        binary /= 10;
        mnozh *= 2; 
    }
    std::cout << des;
    return 0;
}