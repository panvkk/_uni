#include <iostream>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) return 0;

    std::vector<int> a(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> a[i];
    }

    bool possible = false;

    for (int i = 0; i < n; ++i) {
        for (int j = i + 1; j < n; ++j) {
            int target_sum = a[i] + a[j];
            bool valid = true;

            for (int k = 0; k < n; ++k) {
                if (k != i && k != j) {
                    if (a[k] != target_sum) {
                        valid = false;
                        break;
                    }
                }
            }

            if (valid) {
                possible = true;
                break;
            }
        }
        if (possible) break;
    }

    if (possible) {
        std::cout << "YES\n";
    } else {
        std::cout << "NO\n";
    }

    return 0;
}