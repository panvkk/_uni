#include <iostream>
                    
int main()
{
    int n,a=1,b=0,c=0;
    std::cout<< "Введите число: \n";
    std::cin >> n;
    if(n<0)
    {
        a=-1;
        for(int i = -1; i != n+1; i--)
        {
            c=b;
            b=a;
            a = b+c;
        }
        std::cout << a << std::endl;
        exit(0);
    }
    if(n==0)
        std::cout << 0 << std::endl;
    for(int i = 1; i != n-1; i++)
    {
        c=b;
        b=a;
        a = b+c;
    }
    std::cout << a;
    return 0; 
}