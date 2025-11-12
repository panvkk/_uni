#include <fstream>

int main() {
    std::ifstream input("input.txt");
    std::ofstream output("output.txt");

    int n;
    input >> n;
    int* arr = new int[n];

    for (int i = 0; i < n; i++) {
        input >> arr[i];
    }
    for (int i = 0; i < n; i++) {
        if (2*i + 1 < n) {
            if (arr[i] > arr[2*i + 1]) {
                output << "No\n";
                delete[] arr;
                return 0;
            }
        }
        if (2*i + 2 < n) {
            if (arr[i] > arr[2*i + 2]) {
                output << "No\n";
                delete[] arr;
                return 0;
            }
        }
    }

    output << "Yes\n";
    delete[] arr;
    return 0;
}