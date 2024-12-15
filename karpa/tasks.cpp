#include <iostream>
#include <fstream>
#include <string>
#include <ostream>
#include <cmath>

void task_numb_1();
void task_numb_2();
void task_numb_3();
void task_numb_4();
void task_numb_5();
void task_numb_6();
void swap(int&, int&);

int main() {
    srand(time(0));
    task_numb_1();
    return 0;
}

void task_numb_1() {
    int k, d = 100;
    std::cin >> k;
    for(int i = 1; i <= 3; i++){
        std::cout << k / d;
        if(i == 2)
            std::cout << k / d << k / d;
        k -= (k/d)*d;
        d/=10;
    }
}

void task_numb_2() {
    int a, b;
    long long product = 1;
    std::cout << "Введите границы промежутка: ";
    std::cin >> a >> b;
    if(a>b)
        swap(a,b);
    for(int i = a; i <= b; i++)
        product *= i;
    std::cout << "Произведение: " << product;
}

void task_numb_3() {
    int n, counter = 0;
    std::cout << "Введите число: ";
    std::cin >> n;
    int* array = new int[n];
    for(int i = 0; i < n; i++) {
        array[i] = rand()%10;
        std::cout << array[i] << " ";
    }
    std::cout << "\n";
    int min_element = 10, min_element_amount = 0;
    for(int i = 0; i < n; i++) {                // нахожу минимальный элемент
        if(min_element > array[i]) {
            min_element = array[i];
        }
    }
    for(int i = 0; i < n; i++) {                // нахожу повторения минимального элемента, чтобы определить новый массив
        if(min_element == array[i])
            min_element_amount++;
    }
    int* new_array = new int[n-min_element_amount];
    for(int i = 0; i < n; i++) {            // определяю новый массив 
        if(array[i] == min_element)
            continue;
        new_array[counter] = array[i];
        counter++;
    }
    for(int i = 0; i < n-min_element_amount; i++)
        std::cout << new_array[i] << " ";
}

void task_numb_4() {
    int n, m, order = 0;
    std::cout << "Введите число: ";
    std::cin >> n;
    m = n;
    while(m >= 1) {         // определяю количество цифр числа, чтобы задать динамический массив 
        m/=10;
        order++;
    }
    int* array = new int[order+1];
    array[0] = n;
    for(int i = 1; i < order+1; i++) {
        array[i] = (array[i-1]%10)*pow(10, order-1) + (array[i-1]/10);
    }
    for(int i = 0; i < order+1; i++)
        std::cout << array[i] << " ";
}

void task_numb_5() {
    int N;
    std::cout << "Введите N: ";
    std::cin >> N;
    int** matrix = new int*[N];
    for(int i = 0; i < N; i++) {
        matrix[i] = new int[N];
        for(int j = 0; j < N ; j++) {               // инициализирую дин. массив и сразу же вывожу его
            matrix[i][j] = rand()%5;                
            std::cout << matrix[i][j] << " ";
        }
        std::cout << "\n";
    }
    for(int i = 0; i < N; i++) {
        if(matrix[N-1][i]%2 == 1) {
            int product = 1;
            for(int j = 0; j < N; j++)
                product *= matrix[j][i];
            std::cout << "Столбец " << i+1 << ", произведение = " << product << '\n';
        }
    }
}

void task_numb_6() {
    std::ifstream infile("Input.txt");
    std::string line, first_word;
    getline(infile, line);
    auto pos = line.find_first_of(",.!-: ");
    first_word = line.substr(0, pos);           // выделяю первое слово 
    infile.seekg(0);
    while(!infile.eof()) {          // прохожусь по строкам до конца файла
        getline(infile, line);
        std::string new_line, current_word;
        for(int i = 0; i < line.size(); i++) {     // прохожусь по элементам текущей строки
            if(line[i] == ' ' || line[i] == ',' || line[i] == '.' || line[i] == '!' || line[i] == '-' || line[i] == ':') {
                if(!current_word.empty()) {
                    if(current_word == first_word) {            
                        current_word.clear();
                    } else {                        // если текущее слово не равно первому, то закидываю в новую строку
                        for(int l = 0; l < current_word.size(); l++) {
                            new_line.push_back(current_word[l]);
                        }
                        new_line.push_back(' ');
                        current_word.clear();
                    }
                }
            } else 
                current_word.push_back(line[i]);
        }
        if(!current_word.empty()) {             // после цикла последнее слово строки могло остаться в current_word
            for(int l = 0; l < current_word.size(); l++) {
                    new_line.push_back(current_word[l]);
            }
            current_word.clear();
        }
        std::cout << new_line << "\n";
    }
}

void swap(int& first, int& second) {
    int temp = first;
    first = second;
    second = temp;
}