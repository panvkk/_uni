#include <iostream>
#include <fstream>

void swap(std::string*&, std::string*&);
void swap(std::string&, std::string&);
void sorting(std::string*, int);
int wordsQuantity(std::string);
int findArraySize(std::string*, int); 


int main() { 
    std::ifstream FILE("Input.txt");
    std::string* text;
    int lines_quantity = 0;
    while(!FILE.eof()) {
        std::string line;
        std::getline(FILE, line);
        lines_quantity++;
    }
    FILE.seekg(0 , std::ios::beg);
    text = new std::string[lines_quantity];
    for(int i = 0; !FILE.eof(); i++) {
        std::getline(FILE, text[i]);
    }
    // 2 задание
    sorting(text, lines_quantity);

    // 3 задание

    int max_amount_of_words = 0;
    for(int i = 0; i < lines_quantity; i++) {                  //подсчёт наибольшего количества слов
        int current_amount = wordsQuantity(text[i]);
        if(current_amount > max_amount_of_words)
            max_amount_of_words = current_amount;
    } max_amount_of_words++;          //так как мы посчитали пробелы, надо увеличить на 1, чтобы получить количество слов

    std::string** matrix;                                       //новая динамическая матрица
    // matrix = new std::string*[lines_quantity];
    // for(int i = 0; i < lines_quantity; i++) {
    //     matrix[i] = new std::string[max_amount_of_words];
    //     for(int k = 0; k < max_amount_of_words; k++) 
    //         matrix[i][k] = '\0';
    // }
    // for(int l = 0; l < lines_quantity; l++) {                   //заполнение матрицы отдельными словами
    //     int numb_of_word = 0;
    //     std::string word;
    //     for(int i = 0; i <= text[l].size(); i++) {
    //         if(text[l][i] == ' ' || text[l][i] == '\0') {
    //             matrix[l][numb_of_word] = word;
    //             word.clear();
    //             numb_of_word++;
    //         } else {
    //             word.push_back(text[l][i]);
    //         }
    //     }
    // }
    
    // 4 задание
    // int A;
    // std::cin >> A;
    // for(int i = 0; i < lines_quantity; i++) {
    //     for(int k = 0; k < max_amount_of_words; k++) {
    //         if(A > matrix[i][k].size())
    //             continue;
    //         matrix[i][k].erase(A-1, 1);
    //     }
    // }
    // matrix selection sorting (5 задание)
    for(int i = 0; i < lines_quantity-1; i++) {
        if(findArraySize(matrix[i], max_amount_of_words) < findArraySize(matrix[i], max_amount_of_words)) {
            swap(matrix[i], matrix[i+1]);
            i = 0;
        }
    }
    // int max_amo = 0;
    // int numb;
    // for(int l = 0; l < lines_quantity; l++) {
    //     for(int i = l; i < lines_quantity; i++) {
    //         int current_amo = 0;
    //         for(int k = 0; k < max_amount_of_words; k++) {
    //             if(matrix[i][k] != "\0") {
    //                 current_amo++;
    //             }
    //         }
    //         if(current_amo > max_amo)
    //             max_amo = current_amo; numb = i;
    //     }
    //     swap(matrix[l], matrix[numb]);
    // }
    for(int i = 0; i < lines_quantity; i++) {
        for(int k = 0; k < max_amount_of_words; k++) 
            std::cout  << matrix[i][k] << " ";
        std::cout << "\n";
    }
    return 0;
}

int findArraySize(std::string* a, int b) {
    int size = 0;
    for(int m = 0; m < b; m++) {
        if(a[m] != "\0")
            size++;
    }
    return size;
}

void sorting(std::string* t, int num) {
    for(int i = 0; i+1 < num; i++) {
        if(t[i-1].size() < t[i].size()) {
            swap(t[i], t[i+1]);
            i = 0;
        }
    }
}
void swap(std::string& a, std::string& b) {
    std::string temp = a;
    a = b;
    b = temp;
}
void swap(std::string*& a, std::string*& b) {
    std::string* temp = a;
    a = b;
    b = temp;
}
// int wordsQuantity(std::string str) {
//     int quantity = 0;
//     for(int i = 0; i < str.size(); i++) {
//         if(str[i] == ' ')
//             quantity++;
//     }
//     return quantity+1;
// }