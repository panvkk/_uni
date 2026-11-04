#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>

using namespace std;

const int MOD = 1e9 + 7;

vector<long long> count_ways(const vector<int>& rows, int k) {
    vector<long long> dp(k + 1, 0);
    dp[0] = 1;

    for (int len : rows) {
        for (int j = k; j >= 1; --j) {
            long long free_spots = len - (j - 1);

            if (free_spots > 0) {
                dp[j] = (dp[j] + dp[j - 1] * free_spots) % MOD;
            }
        }
    }
    return dp;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    if (k > 2 * n) {
        cout << 0 << endl;
        return 0;
    }

    vector<int> all_diagonals;
    for (int i = 1; i <= n; ++i) all_diagonals.push_back(i);
    for (int i = n - 1; i >= 1; --i) all_diagonals.push_back(i);

    vector<int> black_diagonals;
    vector<int> white_diagonals;

    for (size_t i = 0; i < all_diagonals.size(); ++i) {
        if (i % 2 == 0) {
            black_diagonals.push_back(all_diagonals[i]);
        } else {
            white_diagonals.push_back(all_diagonals[i]);
        }
    }

    sort(black_diagonals.begin(), black_diagonals.end());
    sort(white_diagonals.begin(), white_diagonals.end());

    vector<long long> black_ways = count_ways(black_diagonals, k);
    vector<long long> white_ways = count_ways(white_diagonals, k);

    long long ans = 0;
     for (int i = 0; i <= k; ++i) {
        long long ways = (black_ways[i] * white_ways[k - i]) % MOD;
        ans = (ans + ways) % MOD;
    }

    cout << ans << endl;

    return 0;
}