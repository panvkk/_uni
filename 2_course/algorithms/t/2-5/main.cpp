#include <bits/stdc++.h>

using namespace std;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n;
    cin >> n;
    vector<int> a(n);
    for (int i = 0; i < n; i++) cin >> a[i];

    if (count(a.begin(), a.end(), a[0]) == n) {
        for (int i = 0; i < n; i++)
            cout << 0 << " \n"[i == n-1];
        return 0;
    }

    map<int, int> cnt;
    for (int x : a) cnt[x]++;

    map<int, set<int>> edge_max, edge_min;
    for (int i = 0; i < n; i++) {
        int mx = max(a[i], a[(i+1) % n]);
        int mn = min(a[i], a[(i+1) % n]);
        edge_max[mx].insert(i);
        edge_min[mn].insert(i);
    }

    auto getDist = [&](int pos, const set<int>& edges) -> int {
        if (edges.empty()) return INT_MAX;
        int best = INT_MAX;
        for (int target : {(pos - 1 + n) % n, pos}) {
            auto it = edges.lower_bound(target);
            if (it != edges.end()) best = min(best, *it - target);
            if (it != edges.begin()) best = min(best, target - *prev(it));

            best = min(best, target + n - *edges.rbegin());
            best = min(best, *edges.begin() + n - target);
        }
        return best;
    };

    vector<int> ans(n);

    for (int i = 0; i < n; i++) {
        int v = a[i];
        int k = cnt[v];
        int base = n - k;

        int prev = a[(i - 1 + n) % n];
        int next = a[(i + 1) % n];

        if (prev == v || next == v) {
            ans[i] = base;
        }
        else if (prev <= v && next <= v) {
            ans[i] = base;
        }
        else if (prev >= v && next >= v) {
            ans[i] = base;
        }
        else {
            int d = INT_MAX;

            int badPos = (next > v) ? (i + 1) % n : (i - 1 + n) % n;
            set<int> smallSources;
            for (auto& [val, edges] : edge_max) {
                if (val <= v)
                    for (int e : edges)
                        smallSources.insert(e);
                else break;
            }
            d = min(d, getDist(badPos, smallSources));

            badPos = (prev < v) ? (i - 1 + n) % n : (i + 1) % n;
            set<int> bigSources;
            for (auto it = edge_min.rbegin(); it != edge_min.rend(); ++it) {
                if (it->first >= v)
                    for (int e : it->second)
                        bigSources.insert(e);
                else break;
            }
            d = min(d, getDist(badPos, bigSources));

            ans[i] = base + d;
        }
    }

    for (int i = 0; i < n; i++)
        cout << ans[i] << " \n"[i == n-1];
    return 0;
}