#include <iostream>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

const int INF = 1e9;

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int n, m;
    if (!(cin >> n >> m)) return 0;

    vector<vector<int>> adj(n + 1);
    for (int i = 0; i < m; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
    }

    int min_cycle = INF;

    for (int i = 1; i <= n; ++i) {
        vector<int> dist(n + 1, INF);
        vector<int> parent(n + 1, -1);
        queue<int> q;

        dist[i] = 0;
        q.push(i);

        while (!q.empty()) {
            int u = q.front();
            q.pop();

            if (dist[u] * 2 + 1 >= min_cycle) break;

            for (int v : adj[u]) {
                if (dist[v] == INF) {
                    dist[v] = dist[u] + 1;
                    parent[v] = u;
                    q.push(v);
                } else if (parent[u] != v) {
                    min_cycle = min(min_cycle, dist[u] + dist[v] + 1);

                    if (min_cycle == 3) {
                        cout << 3 << endl;
                        return 0;
                    }
                }
            }
        }
    }

    if (min_cycle == INF) {
        cout << -1 << endl;
    } else {
        cout << min_cycle << endl;
    }

    return 0;
}