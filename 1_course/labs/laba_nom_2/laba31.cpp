#include <iostream>

const int LEN = 10, HIG = 3;

int main()
{
    srand(time(0));
    int mass[LEN][HIG];
    for(int l = 0; l < HIG; l++)
    {   
        for(int i = 0; i < LEN; i++)
        {
            mass[i][l] = rand()%10; 
        }   
    }
    for(int l = 0; l < HIG; l++)
    {   
        for(int i = 0; i < LEN; i++)
        {
            std::cout << mass[i][l] << " "; 
        }
        std::cout << "\n";
    }
    for(int i = 0; i < LEN; i++)
    {   
        bool parity = false;
        for(int l = 0; l < HIG; l++)
        {
            if(mass[i][l]%2 != 0)
                continue;
            else{
                parity = true;
                break;
            }
        }
        if(parity == false)
            std::cout << "Столбец " << i+1 << " - все элементы нечётные.\n";
    }
    return 0;
}