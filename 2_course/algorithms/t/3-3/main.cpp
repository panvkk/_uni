#include <iostream>

using namespace std;

typedef long long ll;
const ll MOD = 1e9 + 7;

ll binpow(ll base, ll exp) {
    ll res = 1;
    base %= MOD;
    while (exp > 0) {
        if (exp % 2 == 1) res = (res * base) % MOD;
        base = (base * base) % MOD;
        exp /= 2;
    }
    return res;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    ll n, m;
    if (!(cin >> n >> m)) return 0;

    if (m < 2) {
        cout << 0 << endl;
        return 0;
    }

    ll first_col = (m % MOD) * ((m - 1) % MOD) % MOD;

    ll m_mod = m % MOD;
    ll base = (m_mod * m_mod) % MOD;
    base = (base - (3 * m_mod) % MOD + 3 + MOD) % MOD;

    ll multiplier = binpow(base, n - 1);
    ll ans = (first_col * multiplier) % MOD;

    cout << ans << endl;

    return 0;
}