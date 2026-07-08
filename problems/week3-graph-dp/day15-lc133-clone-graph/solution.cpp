// ============================================================================
// LC-133: Clone Graph
// Difficulty: Medium
// Priority: P1
// Week 3 / Day 15
// ----------------------------------------------------------------------------
// Given a reference of a node in a connected undirected graph, return a deep
// copy (clone) of the graph. Each node has a val and a list of neighbors.
//
// Constraints:
//   - number of nodes in [0, 100]
//   - 1 <= Node.val <= 100
//   - Node.val unique, no undirected self-loops, no repeated edges
//   - graph connected
//
// Goal: O(V+E) time.
//
// Local I/O format (for test.in):
//   Line 1: n (number of nodes, labeled 1..n) m (number of edges)
//   Next m lines: u v (edge between u and v)
//   Print the clone's adjacency list: for each node i, its neighbors space-separated,
//   one node per line.
// Expected output for test.in: 2 | 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() { val = 0; neighbors = vector<Node*>(); }
    Node(int _val) { val = _val; neighbors = vector<Node*>(); }
    Node(int _val, vector<Node*> _neighbors) { val = _val; neighbors = _neighbors; }
};

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    Node* cloneGraph(Node* node) {
        // Your implementation here.
        return nullptr;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<Node*> nodes(n + 1);
    for (int i = 1; i <= n; ++i) nodes[i] = new Node(i);
    for (int e = 0; e < m; ++e) {
        int u, v; cin >> u >> v;
        nodes[u]->neighbors.push_back(nodes[v]);
        nodes[v]->neighbors.push_back(nodes[u]);
    }
    Node* src = (n >= 1) ? nodes[1] : nullptr;
    Node* cl = sol.cloneGraph(src);
    // Print adjacency of the clone via BFS from cl (val == 1).
    if (!cl) { for (int i = 0; i < n; ++i) cout << "\n"; return 0; }
    unordered_map<int, Node*> seen; seen[cl->val] = cl;
    queue<Node*> q; q.push(cl);
    vector<vector<int>> adj(n + 1);
    while (!q.empty()) {
        Node* c = q.front(); q.pop();
        for (Node* nb : c->neighbors) {
            adj[c->val].push_back(nb->val);
            if (!seen.count(nb->val)) { seen[nb->val] = nb; q.push(nb); }
        }
    }
    for (int i = 1; i <= n; ++i) {
        sort(adj[i].begin(), adj[i].end());
        for (size_t j = 0; j < adj[i].size(); ++j) cout << adj[i][j] << " \n"[j + 1 == adj[i].size()];
        if (adj[i].empty()) cout << "\n";
    }
    return 0;
}

