#include <iostream>
#include <fstream>

int main() {
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int array_size = input.get();

    int* array = new int[array_size];
    for (int i = 0; i < array_size; i++) {
        array[i] = input.get();
    }

    int query_size = input.get();

    int* queries = new int[query_size];
    for (int i = 0; i < query_size; i++) {
        queries[i] = input.get();
    }

    for (int query_index = 0; query_index < query_size; query_index++) {
        int query = queries[query_index];
        int l = 0;
        int r = array_size - 1;

        auto answer = new int[3];
        while (l < r) {
            int m = l + (r - l) / 2;
            if (array[m] > query) {
                l = m + 1;
            } else if (array[m] < query) {
                r = m - 1;
            } else {
                answer[0] = 1;
                answer[1] = m;
                for (int i = m + 1; i < array_size; i++) {
                    if (array[i] > query) {
                        answer[2] = i;
                        break;
                    }
                }
            }
        }
    }


}