// ============================================================================
// LC-207: Course Schedule
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 15
// ----------------------------------------------------------------------------
// There are numCourses courses labeled 0..numCourses-1. prerequisites[i] = [a, b]
// means you must take b before a. Return true if you can finish all courses
// (no cycle in the directed graph).
//
// Constraints:
//   - 1 <= numCourses <= 2000
//   - 0 <= prerequisites.length <= 5000
//
// Goal: O(V+E) time (topological sort / DFS).
//
// Local I/O format (for test.in):
//   Line 1: numCourses m
//   Next m lines: a b (must take b before a)
//   Print 1 if all courses can be finished, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // Your implementation here.
        return false;
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
    cout << (sol.canFinish(nc, pre) ? 1 : 0) << "\n";
    return 0;
}

