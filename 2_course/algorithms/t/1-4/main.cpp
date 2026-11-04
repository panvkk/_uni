#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

const long long INF = 1e18;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(NULL);

    int n, k;
    if (!(cin >> n >> k)) return 0;

    vector<long long> a(n + 1);
    for (int i = 1; i <= n; ++i) {
        cin >> a[i];
    }

    vector<vector<long long>> dp(n + 1, vector<long long>(k + 1, -INF));

    dp[0][0] = 0;

    for (int j = 0; j <= k; ++j) {
        long long max_prev_layer = -INF;

        for (int i = 1; i <= n; ++i) {
            if (j > 0) {
                if (dp[i-1][j-1] != -INF) {
                    max_prev_layer = max(max_prev_layer, dp[i-1][j-1]);
                }

                if (max_prev_layer != -INF) {
                    dp[i][j] = max(dp[i][j], max_prev_layer + a[i]);
                }
            }

            long long prev_step = -INF;

            if (dp[i-1][j] != -INF) {
                prev_step = max(prev_step, dp[i-1][j]);
            }
            if (i >= 2 && dp[i-2][j] != -INF) {
                prev_step = max(prev_step, dp[i-2][j]);
            }

            if (prev_step != -INF) {
                dp[i][j] = max(dp[i][j], prev_step + a[i]);
            }
        }
    }

    long long result = -INF;
    for (int j = 0; j <= k; ++j) {
        result = max(result, dp[n][j]);
    }

    cout << result << endl;

    return 0;
}