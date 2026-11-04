#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

int get_cost(const string& s, int start, const string& target) {
    int diff = 0;
    for (int i = 0; i < 5; ++i) {
        if (s[start + i] != target[i]) {
            diff++;
        }
    }
    return diff;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    string s;
    if (!(cin >> s)) return 0;
    int n = s.length();

    string t1 = "tbank";
    string t2 = "study";

    vector<int> cost_tbank(n - 4);
    vector<int> cost_study(n - 4);

    for (int i = 0; i <= n - 5; ++i) {
        cost_tbank[i] = get_cost(s, i, t1);
        cost_study[i] = get_cost(s, i, t2);
    }

    vector<int> pref_min_tbank(n - 4);
    vector<int> pref_min_study(n - 4);

    pref_min_tbank[0] = cost_tbank[0];
    pref_min_study[0] = cost_study[0];

    for (int i = 1; i <= n - 5; ++i) {
        pref_min_tbank[i] = min(pref_min_tbank[i - 1], cost_tbank[i]);
        pref_min_study[i] = min(pref_min_study[i - 1], cost_study[i]);
    }

    int min_total_changes = 11;

    for (int j = 5; j <= n - 5; ++j) {
        min_total_changes = min(min_total_changes, pref_min_tbank[j - 5] + cost_study[j]);
        min_total_changes = min(min_total_changes, pref_min_study[j - 5] + cost_tbank[j]);
    }

    cout << min_total_changes << endl;

    return 0;
}