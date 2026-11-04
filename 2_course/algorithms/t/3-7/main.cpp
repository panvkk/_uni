#include <bits/stdc++.h>
using namespace std;

typedef long long ll;
typedef unsigned long long ull;
typedef __int128 lll;

int n;
ll a[65];

ll coins(ll v) {
    if (v < 0) return 1e18;
    ll cnt = 0;
    for (int i = n - 1; i >= 0; i--) {
        cnt += v / a[i];
        v %= a[i];
    }
    return cnt;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll x;
    cin >> n >> x;
    for (int i = 0; i < n; i++) cin >> a[i];

    ll ans = LLONG_MAX;

    for (int i = 0; i < n; i++) {
        ll ai = a[i];

        lll yi = ((lll)x + ai - 1) / ai * ai;
        if (yi > (lll)2e18) continue;
        ll y = (ll)yi;
        ll change = y - x;
        ll total = coins(y) + coins(change);
        ans = min(ans, total);
    }

    cout << ans << endl;

    return 0;
}