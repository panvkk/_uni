#include <iostream>
                    // допустим, 81: (8+1)^2=81. Типа такого
int main()
{
    int k;
    std::cout << "Введите предел вычислений: ";
    std::cin >> k;
    for(int j = 1; j<=k; j++)
    {
        int b=0;
        int i=j;
        float m=j;
        while(i>=1)
        {
            b+=(i%10);
            i/=10;
        }
        while(m >= b)
        {
            if(b==1)
                break;
            m/=b;
        }
        if (m==1)
            std::cout << std::endl << j; 
    }
    return 0; 
}