#include <iostream>
#include <string>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    int n;
    if (!(std::cin >> n)) return 0;

    std::string s;
    std::cin >> s;

    for (int i = 0; i < n; ++i) {
        if (s[i] == 'R') {
            std::cout << i << " ";
        }
    }

    std::cout << n;

    for (int i = n - 1; i >= 0; --i) {
        if (s[i] == 'L') {
            std::cout << " " << i;
        }
    }

    std::cout << '\n';

    return 0;
}