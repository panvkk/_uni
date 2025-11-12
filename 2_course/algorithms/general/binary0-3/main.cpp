#include <fstream>
#include <vector>

using namespace std;

int main() {
    ifstream in("bst.in");
    ofstream out("bst.out");

    int n;
    in >> n;

    vector<long long> values(n + 1);
    vector<long long> min_bounds(n + 1);
    vector<long long> max_bounds(n + 1);

    in >> values[1];
    max_bounds[1] = LONG_LONG_MAX;
    min_bounds[1] = LONG_LONG_MIN;

    for (int i = 2; i <= n; i++) {
        int parent;
        char side;
        in >> values[i] >> parent >> side;

        if (side == 'L') {
            max_bounds[i] = values[parent];
            min_bounds[i] = min_bounds[parent];
        } else if (side == 'R') {
            min_bounds[i] = values[parent];
            max_bounds[i] = max_bounds[parent];
        }

        if (min_bounds[i] > values[i] || values[i] >= max_bounds[i]) {
            out << "NO\n";
            return 0;
        }
    }
    out << "YES\n";
    return 0;
}