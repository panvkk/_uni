#include <iostream>

const int MAX = 5, N = MAX+1;

int main()
{
    srand(time(0));
    int mass[MAX][MAX];
    for(int l = 0; l < MAX; l++)        //задаём матрицу
    {   
        for(int i = 0; i < MAX; i++)
        {
            mass[i][l] = rand()%10; 
            std::cout << mass[i][l] << " "; 
        }
        std::cout << "\n";
    }
    int numb = mass[0][0];
    int coord[2] = { 0, 0 };
    for(int l = 0; l < MAX; l++)            //находим минимальное значение
    {   
        for(int i = 0; i < MAX; i++)
        {
            if(numb > mass[i][l])
            {
                numb = mass[i][l];
                coord[0] = i;
                coord[1] = l;
            }
        }   
    }
    std::cout << std::endl << "Минимальное значение\nСтолбец: " << coord[0]+1 << "\nСтрока: " << coord[1]+1 << std::endl;
    int arr[N][N];
    int c1 = 0, c2 = 0;
    for(int l = 0; l < MAX; l++)
    {   
        for(int i = 0; i < MAX; i++)
        {
            arr[c1][c2] = mass[i][l];         // если столбец совпадет со столбцом мин. эл., то заменяем на этот эл.
            if(c1 == coord[0] ){
                arr[c1][c2] = numb;
                if(i == MAX)
                    break;
                i--;
            }
            c1++;
        }
        c1 = 0;
        if(c2 == coord[1])              //если строка совпадает со строкой мин. эл., то заполняем её значением этого элемента
        {
            for(int j = 0; j < N; j++)
            {
                arr[j][c2] = numb;
                if(j == coord[0])
                    arr[j][c2] = numb;
            }
            l--;
        }
        c2++;
    }
    std::cout << "\n";
    for(int l = 0; l < N; l++)
    {   
        for(int i = 0; i < N; i++)
        {
            std::cout << arr[i][l] << " "; 
        }
        std::cout << "\n";
    }
    return 0;
}