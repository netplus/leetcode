// ============================================================================
// LC-542: 01 Matrix
// Difficulty: Medium
// Priority: P1
// Week 3 / Day 16
// ----------------------------------------------------------------------------
// Given an m x n binary matrix, return the distance of the nearest 0 for each
// cell (distance = number of steps, 4-directional).
//
// Constraints:
//   - m == mat.length, n == mat[i].length
//   - 1 <= m, n <= 1e4
//   - 1 <= m*n <= 1e4
//   - mat[i][j] is 0 or 1
//
// Goal: O(m*n) time (multi-source BFS).
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print the distance matrix, one row per line space-separated.
// Expected output for test.in: 0 0 0 | 0 1 0 | 1 2 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        // Your implementation here.
        return {};
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
    auto res = sol.updateMatrix(g);
    for (auto& r : res) for (size_t j = 0; j < r.size(); ++j) cout << r[j] << " \n"[j + 1 == r.size()];
    return 0;
}

