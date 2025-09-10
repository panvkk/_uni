#include <iostream>
                                    // хммм, типа 121 - да, 13431, тоже..
int main()
{
    int a,b=0;
    std::cout << "Введите число: ";
    std::cin >> a;
    int i = a;
    while(i>=1)
    {
        b*=10;
        b+=i%10;
        i/=10;
    }
    if (b==a)
        std::cout << "Число является палиндромом \n";
    else
        std::cout << "Число не является палиндромом \n";
    return 0;
}