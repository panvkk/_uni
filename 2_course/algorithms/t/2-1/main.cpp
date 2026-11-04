#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(NULL);

    std::string s;
    if (!(std::cin >> s)) return 0;

    std::sort(s.begin(), s.end());

    if (s[0] == '0') {
        for (size_t i = 0; i < s.length(); ++i) {
            if (s[i] != '0') {
                std::swap(s[0], s[i]);
                break;
            }
        }
    }

    std::cout << s << std::endl;

    return 0;
}