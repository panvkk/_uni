#include <iostream>

int main()
{
    srand(time(0));
    enum { MAX = 7, PEAK = 10 /* максимально возможный элемент + 1 */ };
    int mass[MAX];
    int arr1[MAX], arr2[MAX];
    for(int l = 0; l <= MAX-1; l++)
    {   
        mass[l] = rand()%PEAK;
        arr1[l] = PEAK+1;
        arr2[l] = 0;
    }
    for(int l = 0; l <= MAX-1; l++)
    {   std::cout << mass[l] << " ";    }
    std::cout << "\n";
    int x = 0, y = 0;
    for(int i = 0; i < MAX; i++)
    {
        bool a = false;
        for(int j = 0; j < MAX; j++)
        {
            if(arr1[j] == mass[i])
            {
                a = true;
                arr2[j]++;
                break;
            }
        }
        if( a == false )
        {
            arr1[x] = mass[i];
            arr2[x]++;
            x++;
        }
    }
    int maximum = 0, numb = 0;
    for(int m = 0; m <= MAX-1; m++)
    {
        if(maximum < arr2[m])
        {
            maximum = arr2[m];
            numb = m;
        }
    }
    std::cout << "Кол-во повторений: " << maximum << "\nЭлемент: " << arr1[numb];
    return 0;
}