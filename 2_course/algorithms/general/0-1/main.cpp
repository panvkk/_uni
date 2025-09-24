#include <iostream>


int upperBound(const int* arr, const int& arraySize, const int& x) {
    int l = 0, r = arraySize;
    while (l < r) {
        int m = (r + l) / 2;
        if (arr[m] > x) {
            r = m;
        } else if (arr[m] <= x) {
            l = m + 1;
        }
    }
    return l;
}

int binarySearch(const int* arr, const int& arraySize, const int& x) {
    int l = 0, r = arraySize - 1;
    int result = arraySize;
    while (l <= r) {
        int m = (r + l) / 2;
        if (arr[m] >= x) {
            if (arr[m] == x)
                result = m;
            r = m - 1;
        } else if (arr[m] < x) {
            l = m + 1;
        }
    }
    return result;
}

void findByQuery(const int* arr, const int& arraySize, const int& query, std::ostream& output) {
    int search = binarySearch(arr, arraySize, query);
    if (search != arraySize) {
        int upper = upperBound(arr, arraySize, query);
        output << "1" << " " << search << " " << upper << "\n";
    } else {
        int upper = upperBound(arr, arraySize, query);
        output << "0" << " " << upper << " " << upper << "\n";
    }
}

int main() {
    int array_size;
    std::cin >> array_size;

    int* array = new int[array_size];
    for (int i = 0; i < array_size; i++) {
        std::cin >> array[i];
    }

    int query_size;
    std::cin >> query_size;

    int* queries = new int[query_size];
    for (int i = 0; i < query_size; i++) {
        std::cin >> queries[i];
    }

    for (int query_index = 0; query_index < query_size; query_index++) {
        int query = queries[query_index];
        findByQuery(array, array_size, query, std::cout);
    }

    delete[] array;
    delete[] queries;
    return 0;
}