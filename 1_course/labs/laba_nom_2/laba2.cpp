#include <iostream>

static int MAX = 20;
void parallax(int, int[]);
void showArr(int[]);

int main()
{
    srand(time(0));
    int mass[MAX];
    for(int l = 0; l <= MAX-1; l++)
    {   mass[l] = rand()%5;    }
    showArr(mass);
    for(int j = 0; j < MAX; j++)
    {
        if(mass[j] == mass[j+1])
            {   parallax(j + 1, mass); MAX--; j--; }
    }
    showArr(mass);
    return 0;
}
void showArr(int* arr)
{
    for(int l = 0; l <= MAX-1; l++)
    {   std::cout << arr[l] << " ";    }
    std::cout << "\n";
}
void parallax(int q, int* arr)
{
    arr[q] = 0;
    for(int i = q; i < MAX; i++)
    {
        std::swap(arr[i], arr[i+1]);
    }
}