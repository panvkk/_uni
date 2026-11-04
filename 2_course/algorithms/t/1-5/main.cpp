#include <iostream>
#include <vector>
#include <queue>

using namespace std;

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int n, q;
    if (!(cin >> n >> q)) return 0;

    vector<vector<int>> adj(n + 1);

    for (int i = 0; i < q; ++i) {
        int l, r;
        cin >> l >> r;
        adj[l - 1].push_back(r);
        adj[r].push_back(l - 1);
    }

    vector<int> dist(n + 1, -1);
    queue<int> q_bfs;

    dist[0] = 0;
    q_bfs.push(0);

    while (!q_bfs.empty()) {
        int u = q_bfs.front();
        q_bfs.pop();

        if (u == n) {
            cout << "Yes" << "\n";
            cout << dist[n] << "\n";
            return 0;
        }

        for (int v : adj[u]) {
            if (dist[v] == -1) {
                dist[v] = dist[u] + 1;
                q_bfs.push(v);
            }
        }
    }

    cout << "No" << "\n";

    return 0;
}