// ============================================================================
// LC-685: Redundant Connection II
// Difficulty: Hard
// Priority: P1
// Week 3 / Day 17
// ----------------------------------------------------------------------------
// A rooted tree of n nodes labeled 1..n has one extra directed edge added.
// The added edge creates either a cycle or a node with two parents. Return the
// edge that can be removed to make a valid rooted tree; if multiple, return the
// last one in the input.
//
// Constraints:
//   - 3 <= n <= 1000
//   - edges.length == n
//   - edges[i] = [u, v] meaning u is parent of v
//
// Goal: O(n) union-find.
//
// Local I/O format (for test.in):
//   Line 1: m (number of edges == n)
//   Next m lines: u v (u is parent of v)
//   Print the redundant edge "u v".
// Expected output for test.in: 2 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> e(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> e[i][0] >> e[i][1];
    auto ans = sol.findRedundantDirectedConnection(e);
    cout << ans[0] << " " << ans[1] << "\n";
    return 0;
}

