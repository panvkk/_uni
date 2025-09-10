#include <iostream>
#include <fstream>

class Ar2020 {
    int* M;
    int quantity;
public: 

    Ar2020() : M(nullptr), quantity(0) 
        { std::cerr << "Создан пустой массив."; }
    
    Ar2020(const Ar2020& other) : quantity(other.quantity) {
        M = new int[other.quantity];
        for(int i = 0; i < other.quantity; i++)
            M[i] = other.M[i];
        std::cerr << "Вызван конструктор копирования.";
    }
    Ar2020(int& count) : quantity(count) {
        M = new int[count];
        for(int i = 0; i< count; i++)
            M[i] = 0;
    }

    ~Ar2020() {
        delete[] M;
        std::cerr << "Вызван деструктор.";
    }

//  Перегрузка операторов

    Ar2020& operator=(const Ar2020& other) {
        // if(this == *other) 
        //     return *this;
        delete[] M;
        quantity = other.quantity;
        M = new int[other.quantity];
        for(int i = 0; i < other.quantity; i++)
            M[i] = other.M[i];
        return *this;
    }

    int& operator[](const int& index) {
        try {
            return M[index];
        } catch(...) {
            std::cerr << "Индекс не подходит.";
        }
    }
    
    friend std::istream& operator>>(std::istream& in, Ar2020& array) {
        for(int i = 0; i < array.quantity; i++) 
            in >> array.M[i];
        return in;
    }

    friend std::ostream& operator>>(std::ostream& in, const Ar2020& array) {
        for(int i = 0; i < array.quantity; i++) 
            in << array.M[i] << ' ';
        return in;
    }

    Ar2020 operator+(const Ar2020& right) {
        Ar2020 result;
        for(int i = 0; i < right.quantity; i++) {
            result.M[i] = M[i] + right.M[i];
        }
        return result;
    }

    int Sum() {
        int sum = 0;
        for(int i = 0; i < quantity; i++) {
            sum += M[i]; 
        }
        double middle = sum/quantity;
        sum = 0;
        for(int i = 0; i < quantity; i++) {
            if(M[i] < middle) 
                sum += M[i];
        }
        return sum;
    }

    
};

