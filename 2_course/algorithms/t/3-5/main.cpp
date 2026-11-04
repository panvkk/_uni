#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;
    string s;
    cin >> s;

    vector<vector<int>> dp(n, vector<int>(n, INF));

    char toi[] = {'T', 'O', 'I'};

    for (int i = 0; i < n; ++i) {
        bool is_toi = (s[i] == 'T' || s[i] == 'O' || s[i] == 'I');
        dp[i][i] = (is_toi ? 0 : 1);
    }

    auto get_dp = [&](int i, int j) {
        if (i > j) return 0;
        return dp[i][j];
    };

    for (int len = 2; len <= n; ++len) {
        for (int i = 0; i <= n - len; ++i) {
            int j = i + len - 1;

            dp[i][j] = min(get_dp(i + 1, j) + 1, get_dp(i, j - 1) + 1);

            int min_match_cost = 2;
            for (char c : toi) {
                int current_cost = (s[i] == c ? 0 : 1) + (s[j] == c ? 0 : 1);
                if (current_cost < min_match_cost) {
                    min_match_cost = current_cost;
                }
            }

            dp[i][j] = min(dp[i][j], get_dp(i + 1, j - 1) + min_match_cost);
        }
    }

    cout << dp[0][n - 1] << endl;

    return 0;
}