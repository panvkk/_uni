#include <iostream>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    long long n, k;
    if (!(std::cin >> n >> k)) return 0;

    long long left = 1;
    long long right = n;

    for (long long rem = n; rem >= 1; --rem) {
        if (k >= rem - 1 && rem - 1 > 0) {
            std::cout << right << (rem == 1 ? "" : " ");
            k -= (rem - 1);
            right--;
        } else {
            std::cout << left << (rem == 1 ? "" : " ");
            left++;
        }
    }
    std::cout << "\n";

    return 0;
}