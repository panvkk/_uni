#include <iostream>
#include <string>
#include <map>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) return 0;

    std::map<std::string, int> counts;
    int max_freq = 0;

    for (int i = 0; i < n; ++i) {
        std::string s;
        std::cin >> s;
        int freq = ++counts[s];
        if (freq > max_freq) {
            max_freq = freq;
        }
    }

    for (const auto& [str, freq] : counts) {
        if (freq == max_freq) {
            std::cout << str << "\n";
        }
    }

    return 0;
}