#include <iostream>
#include <vector>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n, t;
    if (!(std::cin >> n >> t)) return 0;

    std::vector<int> rows(n, 0);
    std::vector<int> cols(n, 0);
    int diag1 = 0;
    int diag2 = 0;

    int win_round = -1;

    for (int round = 1; round <= t; ++round) {
        int a;
        std::cin >> a;

        if (win_round != -1) continue;

        int val = a - 1;
        int r = val / n;
        int c = val % n;

        rows[r]++;
        cols[c]++;

        if (r == c) {
            diag1++;
        }
        if (r + c == n - 1) {
            diag2++;
        }

        if (rows[r] == n || cols[c] == n || diag1 == n || diag2 == n) {
            win_round = round;
        }
    }

    std::cout << win_round << "\n";

    return 0;
}