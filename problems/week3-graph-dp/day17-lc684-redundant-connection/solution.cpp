// ============================================================================
// LC-684: Redundant Connection
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 17
// ----------------------------------------------------------------------------
// A tree of n nodes labeled 1..n has n-1 edges; one extra edge is added creating
// a cycle. Return the edge that can be removed so the result is a valid tree. If
// multiple answers, return the last one in the input.
//
// Constraints:
//   - number of nodes n in [3, 1000]
//   - edges.length == n
//   - edges[i].length == 2
//   - 1 <= ai < bi <= ai, all pairs unique
//
// Goal: O(n * alpha(n)) union-find.
//
// Local I/O format (for test.in):
//   Line 1: m (number of edges == n)
//   Next m lines: u v
//   Print the redundant edge "u v".
// Expected output for test.in: 1 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
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
    auto ans = sol.findRedundantConnection(e);
    cout << ans[0] << " " << ans[1] << "\n";
    return 0;
}

