#include <iostream>

int main() {
    int n, k;
    std::cin >> n >> k;

}

int req(int n, int k) {
    if (n == 1) return 2;
    return req(n - 1, k);
}