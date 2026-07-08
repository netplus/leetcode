// ============================================================================
// LC-417: Pacific Atlantic Water Flow
// Difficulty: Medium
// Priority: P1
// Week 3 / Day 16
// ----------------------------------------------------------------------------
// Given an m x n matrix heights where heights[r][c] is the height above sea
// level, water can flow from a cell to a neighbor with height <= current. The
// Pacific touches the left/top edges, the Atlantic the right/bottom. Return a
// list of grid coordinates from which water can flow to both oceans.
//
// Constraints:
//   - m == heights.length, n == heights[i].length
//   - 1 <= m, n <= 200
//   - 0 <= heights[r][c] <= 1e5
//
// Goal: O(m*n) time.
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: n space-separated integers per row
//   Print each coordinate "r c" on its own line, sorted.
// Expected output for test.in: 0 4 | 1 3 | 1 4 | 2 2 | 3 2 | 4 0 | 4 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
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
    auto res = sol.pacificAtlantic(g);
    sort(res.begin(), res.end());
    for (auto& c : res) cout << c[0] << " " << c[1] << "\n";
    return 0;
}

