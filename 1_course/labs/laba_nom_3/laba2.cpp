#include <iostream>
#include <vector>

const int MAX = 20;
static int amountElements = MAX;
// void parallax(int, int[]);
void showArr(std::vector<int>);
std::vector<int> myErase(std::vector<int>, int);
// bool review(int*);

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
void parallax(int q, int* arr)
{
    arr[q] = 0;
    for(int i = q; i < MAX; i++)
    {
        std::swap(arr[i], arr[i+1]);
    }
} 

std::vector<int> myErase(std::vector<int> array, int position)
{
    std::vector<int> newArray(amountElements);
    for (int i = 0; i < amountElements; i++) {
        for (int k = 0; k < amountElements+1; k++) {
            if (k == position) {
                k++;
                std::cout << array[k] << "\n";
            }
            newArray[i] = array[k];
        }
    }
    return newArray;
}