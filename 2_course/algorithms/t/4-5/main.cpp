#include <iostream>
#include <vector>
#include <algorithm>

long long get_max_Y(long long X, long long a) {
    if (X >= 0) {
        return X / a;
    } else {
        return (X - a + 1) / a;
    }
}

long long get_min_Y(long long X, long long a) {
    long long P = -a;
    long long N = -X;
    if (N >= 0) {
        return (N + P - 1) / P;
    } else {
        return N / P;
    }
}

long long count_pairs(long long X, const std::vector<long long>& A) {
    long long total_ordered = 0;
    int n = A.size();

    for (int i = 0; i < n; ++i) {
        if (A[i] > 0) {
            long long Y_max = get_max_Y(X, A[i]);
            auto it = std::upper_bound(A.begin(), A.end(), Y_max);
            total_ordered += (it - A.begin());
        } else if (A[i] < 0) {
            long long Y_min = get_min_Y(X, A[i]);
            auto it = std::lower_bound(A.begin(), A.end(), Y_min);
            total_ordered += (A.end() - it);
        } else {
            if (X >= 0) {
                total_ordered += n;
            }
        }
    }

    for (int i = 0; i < n; ++i) {
        if ((__int128)A[i] * A[i] <= X) {
            total_ordered--;
        }
    }

    return total_ordered / 2;
}

int main() {
    std::ios_base::sync_with_stdio(false);
    std::cin.tie(nullptr);

    int n;
    long long k;
    if (!(std::cin >> n >> k)) return 0;

    std::vector<long long> A(n);
    for (int i = 0; i < n; ++i) {
        std::cin >> A[i];
    }

    std::sort(A.begin(), A.end());

    long long low = -2000000000000000000LL;
    long long high = 2000000000000000000LL;
    long long ans = high;

    while (low <= high) {
        long long mid = low + (high - low) / 2;
        if (count_pairs(mid, A) >= k) {
            ans = mid;
            high = mid - 1;
        } else {
            low = mid + 1;
        }
    }

    std::cout << ans << "\n";

    return 0;
}