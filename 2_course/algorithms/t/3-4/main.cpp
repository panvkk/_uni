#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    if (!(cin >> n)) return 0;

    if (n < 2) {
        cout << 0 << endl;
        return 0;
    }

    vector<long long> p(n);
    for (int i = 0; i < n; i++) {
        cin >> p[i];
    }

    vector<long long> left_max(n, 0);
    long long min_price = p[0];
    for (int i = 1; i < n; i++) {
        min_price = min(min_price, p[i]);
        left_max[i] = max(left_max[i - 1], p[i] - min_price);
    }

    vector<long long> right_max(n, 0);
    long long max_price = p[n - 1];
    for (int i = n - 2; i >= 0; i--) {
        max_price = max(max_price, p[i]);
        right_max[i] = max(right_max[i + 1], max_price - p[i]);
    }

    long long total_max_profit = 0;
    for (int i = 0; i < n; i++) {
        total_max_profit = max(total_max_profit, left_max[i] + right_max[i]);
    }

    cout << total_max_profit << endl;

    return 0;
}