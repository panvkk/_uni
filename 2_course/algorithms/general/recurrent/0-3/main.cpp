#include <iostream>
#include <vector>

using namespace std;

constexpr int MOD = 1000000007;

long long modPow(long long a, long long b) {
    long long result = 1;
    while(b > 0) {
        if((b % 2) == 1)
            result = (result*a) % MOD;
        a = (a*a) % MOD;
        b /= 2;
    }
    return result;
}

long long C(int n, int k,const vector<long long>& fact) {
    if (k < 0 || k > n)
        return 0;
    long long result = fact[n];
    result = (result * modPow(fact[k], MOD - 2)) % MOD;
    result = (result * modPow(fact[n-k], MOD-2)) % MOD;
    return result;
}

int main() {
    int n, k;
    cin >> n >> k;
    vector<long long> fact(n+1);
    fact[0] = 1;
    for(int i = 1; i <= n; i++)
        fact[i] = (fact[i-1]*i) % MOD;
    long long ans = C(n, k, fact) % MOD;
    cout << ans << endl;
    return 0;
}