#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

void solve() {
    string s;
    cin >> s;
    long long n = s.length();

    long long max_l = 0;
    long long current_l = 0;

    string double_s = s + s;
    for (char c : double_s) {
        if (c == '1') {
            current_l++;
            max_l = max(max_l, current_l);
        } else {
            current_l = 0;
        }
    }

    if (max_l > n) {
        cout << n * n << "\n";
    } else {
        long long x = max_l + 1;
        long long h = x / 2;
        long long w = x - h;
        cout << h * w << "\n";
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int t;
    if (!(cin >> t)) return 0;
    while (t--) {
        solve();
    }
    return 0;
}