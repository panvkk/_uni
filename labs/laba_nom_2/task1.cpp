#include <iostream>

const int MAX = 7;
// enum coord { A, B , C , D, E, F , G}
bool review(int[], int, int);

int main()
{
    // srand(time(0));
    int mass[MAX][MAX];
    // for(int l = 0; l < MAX; l++)
    // {   
    //     for(int i = 0; i < MAX; i++)
    //     {
    //         mass[i][l] = 0; 
    //     }   
    // }
    // for(int l = 0; l < MAX; l++)
    // {   
    //     for(int i = 0; i < MAX; i++)
    //     {
    //         std::cout << mass[i][l] << " "; 
    //     }
    //     std::cout << std::endl;   
    // }
    // int amount = rand()%5;
    // int k = 0;
    // do{
    //     int lenght = rand()%4;
    //     bool vertical = rand()%2;
    //     int c1 = rand()%MAX;
    //     int c2 = rand()%MAX;
        if( review( mass, c1, c2) )
    //     {
    //         mass[c1][c2] = 1;
    //         if(vertical)
    //         {
    //             for(int l = 1; l < lenght; l++)
    //             {
    //                 if( mass[c1][c2 + 1] == 0 && mass[c1][c2 + 2] == 0)
    //                 {
    //                     mass[c1][c2+l] = 1;
    //                 }
    //                 else 
    //                 {
    //                     break;
    //                 }
    //             }
    //         }
    //         else
    //         {
    //             for(int l = 1; l < lenght; l++)
    //             {
    //                 if( mass[c1+1][c2] == 0 && mass[c1+1][c2] == 0)
    //                 {
    //                     mass[c1+l][c2] = 1;
    //                 }
    //                 else 
    //                 {
    //                     break;
    //                 }
    //             }
    //         }
    //     }
    // }while(k <= amount);
    // for(int l = 0; l < MAX; l++)
    // {   
    //     for(int i = 0; i < MAX; i++)
    //     {
    //         std::cout << mass[i][l] << " "; 
    //     }
    //     std::cout << std::endl;   
    // }
    return 0;
}
bool review(int** ptr, int q, int w)
{
    if( ptr[q][w] || ptr[q-1][w] || ptr[q-1][w] || ptr[q-1][w-1] || ptr[q][w-1] || ptr[q+1][w-1] || ptr[q-1][w+1] || ptr[q][w+1] || ptr[q+1][w+1])
        return 0;
    else
        return 1;
}