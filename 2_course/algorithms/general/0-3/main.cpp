#include <iostream>


int req(int n, int k) {
    if (n == k || k == 0)
        return 1;
    return req(n - 1, k) + req(n - 1, k - 1);
}

int main() {
    int n, k;
    std::cin >> n >> k;

    for (int i = 0; i < n; i++) {

    }

    int answer = req(n, k);
    std::cout << answer << std::endl;
    return 0;
}