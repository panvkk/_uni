#include <iostream>

bool func(int l,int n)
{
    int x,y,z;
    z=(l%10);
    l/=10;
    y=(l%10);
    l/=10;
    x=(l%10);
    l/=10;
    if(((x*x*x)+(y*y*y)+(z*z*z))==n)
        return true;
    else
        return false;
}

int main()
{
    int m;
    std::cout << "Vvedite chislo: ";
    std::cin >> m;
    for(int j = 100; j<=999; j++)                   // цикл для определения меньшего числа
    {
        if(func(j,m) == true)
        {
            std::cout << "min: " << j << std::endl;
            break;
        }
        else
            continue;
    }
    for(int j = 999; j>=100; j--)                   // цикл для определения большего числа
    {
        if(func(j,m) == true)
        {
            std::cout << "max: " << j << std::endl;
            break;
        }
        else
            continue;

    }
    return 0; 
}