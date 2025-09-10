#include <iostream>
#include <vector>

const int MAX = 20;
static int amountElements = MAX;
void showArr(std::vector<int>);

int main()
{
    srand(time(0));
    std::vector<int> mass(MAX);
    for(int l = 0; l < MAX; l++)
    {   mass[l] = rand()%5;    }
    showArr(mass);
    // while(true)
    // {
        for(int j = 0; j+1 < amountElements; j++)
        {
            if(mass[j] == mass[j+1])
                {  --amountElements; mass = myErase(mass, j); }
        }
    //     if (suit == true)
    //         break;
    // }
    showArr(mass);
    return 0;
}
void showArr(std::vector<int> arr)
{
    for(int l = 0; l <= MAX-1; l++)
    {   std::cout << arr[l] << " ";    }
    std::cout << "\n";
}