// ============================================================================
// LC-200: Number of Islands
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 15
// ----------------------------------------------------------------------------
// Given an m x n 2D binary grid of '1' (land) and '0' (water), return the number
// of islands. An island is formed by connecting adjacent lands horizontally or
// vertically. All four edges are surrounded by water.
//
// Constraints:
//   - m == grid.length
//   - n == grid[i].length
//   - 1 <= m, n <= 300
//   - grid[i][j] is '0' or '1'
//
// Goal: O(m*n) time, O(m*n) space (DFS/BFS).
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: each row as a string of n chars ('0'/'1')
//   Print the number of islands.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int numIslands(vector<vector<char>>& grid) {
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
    vector<vector<char>> g(m, vector<char>(n));
    for (int i = 0; i < m; ++i) {
        string row; cin >> row;
        for (int j = 0; j < n; ++j) g[i][j] = row[j];
    }
    cout << sol.numIslands(g) << "\n";
    return 0;
}

