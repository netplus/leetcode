// ============================================================================
// LC-64: Minimum Path Sum
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 19
// ----------------------------------------------------------------------------
// Given an m x n grid of non-negative numbers, find a path from top-left to
// bottom-right which minimizes the sum of numbers along the path (move only right
// or down). Return the minimum sum.
//
// Constraints:
//   - m == grid.length, n == grid[i].length
//   - 1 <= m, n <= 200
//   - 0 <= grid[i][j] <= 200
//
// Goal: O(m*n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the minimum path sum.
// Expected output for test.in: 7
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int minPathSum(vector<vector<int>>& grid) {
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
    cout << sol.minPathSum(g) << "\n";
    return 0;
}

