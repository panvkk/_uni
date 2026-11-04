#include <bits/stdc++.h>
using namespace std;

struct Node {
    long long key, lazy = 0;
    int prio;
    vector<int> ids;
    Node *l = nullptr, *r = nullptr;
    Node(long long k, vector<int> i) : key(k), prio(rand()), ids(move(i)) {}
};

void push(Node* t) {
    if (!t || !t->lazy) return;
    t->key += t->lazy;
    if (t->l) t->l->lazy += t->lazy;
    if (t->r) t->r->lazy += t->lazy;
    t->lazy = 0;
}

void split(Node* t, long long k, Node*& l, Node*& r) {
    if (!t) { l = r = nullptr; return; }
    push(t);
    if (t->key <= k) { split(t->r, k, t->r, r); l = t; }
    else { split(t->l, k, l, t->l); r = t; }
}

void merge(Node*& t, Node* l, Node* r) {
    push(l); push(r);
    if (!l || !r) t = l ? l : r;
    else if (l->prio > r->prio) { merge(l->r, l->r, r); t = l; }
    else { merge(r->l, l, r->l); t = r; }
}

void ins(Node*& root, long long key, vector<int> ids) {
    Node *l, *m, *r;
    split(root, key, l, r);
    split(l, key - 1, l, m);
    if (m) m->ids.insert(m->ids.end(), ids.begin(), ids.end());
    else m = new Node(key, move(ids));
    merge(l, l, m); merge(root, l, r);
}

void extract(Node* t, vector<pair<long long, vector<int>>>& out) {
    if (!t) return;
    push(t);
    extract(t->l, out);
    out.emplace_back(t->key, move(t->ids));
    extract(t->r, out);
    delete t;
}

int main() {
    ios::sync_with_stdio(false); cin.tie(nullptr);
    srand(chrono::steady_clock::now().time_since_epoch().count());

    int n, q; cin >> n >> q;
    string s; cin >> s;

    vector<array<long long, 3>> ops(q);
    for (int i = 0; i < q; i++) {
        cin >> ops[i][0] >> ops[i][1];
        if (ops[i][0] == 1) cin >> ops[i][2];
    }

    Node* root = nullptr;
    vector<char> ans(q);

    for (int i = q - 1; i >= 0; i--) {
        if (ops[i][0] == 2) {
            ins(root, ops[i][1], {i});
        } else {
            long long l = ops[i][1], r = ops[i][2], len = r - l + 1;
            Node *L, *M, *R, *tmp;
            split(root, l + 2*len - 1, tmp, R);
            split(tmp, l - 1, L, M);
            if (R) R->lazy -= len;
            if (M) {
                vector<pair<long long, vector<int>>> data;
                extract(M, data);
                merge(root, L, R);
                for (auto& [p, ids] : data)
                    ins(root, l + (p - l) / 2, move(ids));
            } else merge(root, L, R);
        }
    }

    vector<pair<long long, vector<int>>> data;
    extract(root, data);
    for (auto& [p, ids] : data)
        for (int id : ids) ans[id] = s[p - 1];

    for (int i = 0; i < q; i++)
        if (ops[i][0] == 2) cout << ans[i] << '\n';
}