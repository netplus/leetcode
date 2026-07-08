// ============================================================================
// LC-210: Course Schedule II
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 21
// ----------------------------------------------------------------------------
// Like Course Schedule, but return any valid ordering of courses to take (a
// topological order). If impossible, return an empty array.
//
// Constraints:
//   - 1 <= numCourses <= 2000
//   - 0 <= prerequisites.length <= 5000
//
// Goal: O(V+E) time (topological sort).
//
// Local I/O format (for test.in):
//   Line 1: numCourses m
//   Next m lines: a b (b before a)
//   Print the ordering space-separated, or -1 if impossible.
// Expected output for test.in: 0 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nc, m; if (!(cin >> nc >> m)) return 0;
    vector<vector<int>> pre(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> pre[i][0] >> pre[i][1];
    auto ans = sol.findOrder(nc, pre);
    if (ans.empty()) cout << -1 << "\n";
    else for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

