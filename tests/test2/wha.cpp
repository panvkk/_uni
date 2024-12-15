#include <iostream>

int main() {
    int height, lenght;
    std::cout << "Введите 2 размерности матрицы: \n";
    std::cin >> lenght >> height;

    int** matrix = new int* [lenght];
    for(int i = 0; i < lenght; i++) {
        matrix[i] = new int [height]; 
        for(int k = 0; k < height; k++) {
            matrix[i][k] = (rand()%5)-(rand()%10);
        }
    }
    for(int i = 0; i < lenght; i++) {
        for(int k = 0; k < height; k++) {
            std::cout << matrix[i][k] << " ";
        }
        std::cout << "\n";
    }
    return 0;
}