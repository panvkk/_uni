#include <iostream>

int main()
{
    int k;
    std::cout << "Vvedite predel vicheslenii: ";
    std::cin >> k;
    for(int j = 1; j<=k; j++)           // определяем цикл, который будет считать варианты до определённого предела
    {
        int b=0;
        int i=j;
        float m=j;
        while(i>=1)                     // считаем сумму цифр проверяемого числа
        {
            b+=(i%10);
            i/=10;
        }
        while(m >= b)                   // если проверяемое число делится на сумму его цифр без остатка вплоть до 1, то условие выполняется
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