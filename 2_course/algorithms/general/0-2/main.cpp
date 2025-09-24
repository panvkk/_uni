#include <iostream>

int main() {
    int array_size;
    std::cin >> array_size;
    int* array = new int[array_size];
    for (int i = 0; i < array_size; i++) {
        std::cin >> array[i];
    }

    if (array_size < 3) {
        std::cout << "-1" << std::endl;
        return 0;
    }

    int* prev = new int[array_size];
    prev[array_size - 1] = -1;
    prev[array_size - 2] = -1;
    prev[array_size - 3] = array_size - 1;

    int c = array[array_size - 1];
    int b = array[array_size - 2];
    int a = array[array_size - 1] + array[array_size - 3];

    for (int i  = array_size - 4; i >= 0; i--) {
        int curr = array[i] + std::max(b,c );
        if ( b > c ) {
            prev[i] = i + 2;
        } else {
            prev[i] = i + 3;
        }
        c = b;
        b = a;
        a = curr;
    }

    std::cout << a << std::endl << "1 ";

    for (int i = 0; prev[i] != -1; i = prev[i]) {
        std::cout << prev[i] + 1 << " ";
    }
    delete[] prev;
    delete[] array;
}