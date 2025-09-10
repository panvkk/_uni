#include <iostream>

int main() {
    
    // int HIG, LEN;
    // std::cout << "Введите 2 размерности матрицы: \n";
    // std::cin >> LEN >> HIG;

    // int** matrix = new int*[LEN];
    // for(int i = 0; i < LEN; i++) {
    //     matrix[i] = new int[HIG]; 
    //     for(int k = 0; k < HIG; k++) {
    //         matrix[i][k] = (rand()%5)-(rand()%10);
    //     }
    // }
    // FILE.open("input",ios::in | ios::binary); 
    std::string str = "I need to understand how it works";
    size_t A;
    std::cin >> A;
    str.erase(str.begin()+A);
    std::cout << str << "\n" << str[1];
    return 0;
}