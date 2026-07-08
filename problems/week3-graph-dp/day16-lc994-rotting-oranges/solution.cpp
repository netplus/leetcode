// ============================================================================
// LC-994: Rotting Oranges
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 16
// ----------------------------------------------------------------------------
// Given an m x n grid with 0 (empty), 1 (fresh), 2 (rotten), every minute any
// fresh orange adjacent (4-dir) to a rotten one rots. Return the minimum number
// of minutes until no fresh orange remains, or -1 if impossible.
//
// Constraints:
//   - m == grid.length, n == grid[i].length
//   - 1 <= m, n <= 10
//   - grid[i][j] in {0,1,2}
//
// Goal: O(m*n) time (multi-source BFS).
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the minimum minutes, or -1.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int orangesRotting(vector<vector<int>>& grid) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.orangesRotting(g) << "\n";
    return 0;
}

