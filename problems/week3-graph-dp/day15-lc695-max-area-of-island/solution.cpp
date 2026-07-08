// ============================================================================
// LC-695: Max Area of Island
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 15
// ----------------------------------------------------------------------------
// Given an m x n binary matrix grid of 0 (water) and 1 (land), return the
// maximum area of an island. If no island, return 0.
//
// Constraints:
//   - m == grid.length, n == grid[i].length
//   - 1 <= m, n <= 50
//   - grid[i][j] is 0 or 1
//
// Goal: O(m*n) time.
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the maximum island area.
// Expected output for test.in: 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int maxAreaOfIsland(vector<vector<int>>& grid) {
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
    cout << sol.maxAreaOfIsland(g) << "\n";
    return 0;
}

