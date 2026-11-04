#include <iostream>
#include <vector>
#include <numeric>

using namespace std;

struct DSU {
    vector<int> parent;

    void init(int n) {
        parent.resize(n + 1);
        iota(parent.begin(), parent.end(), 0);
    }

    int find(int i) {
        if (parent[i] == i)
            return i;
        return parent[i] = find(parent[i]);
    }

    bool unite(int i, int j) {
        int root_i = find(i);
        int root_j = find(j);
        if (root_i != root_j) {
            parent[root_i] = root_j;
            return true;
        }
        return false;
    }
};

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    DSU dsu[10];
    for (int i = 1; i <= 9; ++i) {
        dsu[i].init(n);
    }

    long long current_mst_weight = 10LL * n - 10;

    auto add_edge = [&](int u, int v, int w) {
        for (int x = w; x <= 9; ++x) {
            if (dsu[x].unite(u, v)) {
                current_mst_weight--;
            }
        }
    };

    for (int i = 0; i < n - 1; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        add_edge(u, v, w);
    }

    for (int i = 0; i < q; ++i) {
        int u, v, w;
        cin >> u >> v >> w;
        add_edge(u, v, w);
        cout << current_mst_weight << "\n";
    }

    return 0;
}