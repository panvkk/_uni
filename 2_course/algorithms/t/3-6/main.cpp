#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

struct Candidate {
    int x, y, id;
    long long diff;
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int a, b, n;
    if (!(cin >> a >> b >> n)) return 0;

    vector<Candidate> candidates(n);
    for (int i = 0; i < n; ++i) {
        cin >> candidates[i].x >> candidates[i].y;
        candidates[i].id = i;
        candidates[i].diff = (long long)candidates[i].x - candidates[i].y;
    }

    sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
        return a.diff > b.diff;
    });

    vector<long long> pref(n + 2, 0);
    vector<long long> suff(n + 2, 0);

    if (a > 0) {
        priority_queue<int, vector<int>, greater<int>> pq;
        long long current_sum = 0;
        for (int i = 1; i <= n; ++i) {
            int val = candidates[i-1].x;
            pq.push(val);
            current_sum += val;
            if (pq.size() > a) {
                current_sum -= pq.top();
                pq.pop();
            }
            if (pq.size() == a) {
                pref[i] = current_sum;
            } else {
                pref[i] = -1;
            }
        }
    } else {
        fill(pref.begin(), pref.end(), 0);
    }

    if (b > 0) {
        priority_queue<int, vector<int>, greater<int>> pq;
        long long current_sum = 0;
        for (int i = n; i >= 1; --i) {
            int val = candidates[i-1].y;
            pq.push(val);
            current_sum += val;
            if (pq.size() > b) {
                current_sum -= pq.top();
                pq.pop();
            }
            if (pq.size() == b) {
                suff[i] = current_sum;
            } else {
                suff[i] = -1;
            }
        }
    } else {
        fill(suff.begin(), suff.end(), 0);
    }

    long long max_total = 0;

    for (int i = 0; i <= n; ++i) {
        if (pref[i] != -1 && suff[i+1] != -1) {
            max_total = max(max_total, pref[i] + suff[i+1]);
        }
    }

    cout << max_total << endl;

    return 0;
}