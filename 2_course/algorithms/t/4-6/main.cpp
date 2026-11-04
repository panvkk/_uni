#include <iostream>
#include <vector>
#include <algorithm>

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    if (!(std::cin >> n)) return 0;

    int total_players = 1 << n;

    std::vector<std::vector<long long>> c(total_players, std::vector<long long>(n + 1, 0));

    for (int i = 0; i < total_players; ++i) {
        for (int j = 1; j <= n; ++j) {
            std::cin >> c[i][j];
        }
    }
    std::vector<long long> dp(total_players, 0);

    for (int d = 1; d <= n; ++d) {
        int block_size = 1 << d;
        int half_size = 1 << (d - 1);

        for (int i = 0; i < total_players; i += block_size) {
            int L_start = i;
            int R_start = i + half_size;

            long long bestL = 0;
            for (int q = L_start; q < R_start; ++q) {
                bestL = std::max(bestL, dp[q] + c[q][d - 1]);
            }

            long long bestR = 0;
            for (int q = R_start; q < i + block_size; ++q) {
                bestR = std::max(bestR, dp[q] + c[q][d - 1]);
            }

            for (int p = L_start; p < R_start; ++p) {
                dp[p] += bestR;
            }
            for (int p = R_start; p < i + block_size; ++p) {
                dp[p] += bestL;
            }
        }
    }
    long long ans = 0;
    for (int p = 0; p < total_players; ++p) {
        ans = std::max(ans, dp[p] + c[p][n]);
    }

    std::cout << ans << "\n";

    return 0;
}