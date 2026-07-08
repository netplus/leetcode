// ============================================================================
// LC-304: Range Sum Query 2D - Immutable
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 4
// ----------------------------------------------------------------------------
// Given a 2D matrix, handle queries sumRegion(row1,col1,row2,col2) returning
// the sum of elements inside that rectangle (inclusive).
// (LC class NumMatrix; here modeled as Solution with build() + sumRegion().)
//
// Constraints:
//   - m == matrix.length, n == matrix[i].length
//   - 1 <= m, n <= 200
//   - -1e5 <= matrix[i][j] <= 1e5
//   - 0 <= row1 <= row2 < m, 0 <= col1 <= col2 < n
//
// Goal: O(mn) build, O(1) query.
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: n space-separated integers
//   Line: q (number of queries)
//   Next q lines: row1 col1 row2 col2
//   Print each region sum on its own line.
// Expected output for test.in: 8
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int sumRegion(int row1, int col1, int row2, int col2) {
        // Your implementation here.
        return 0;
    }
    void build(vector<vector<int>>& matrix) {
        // TODO: precompute 2D prefix sums into a member variable.
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> mat(m, vector<int>(n));
    for (int i = 0; i < m; ++i)
        for (int j = 0; j < n; ++j) cin >> mat[i][j];
    sol.build(mat);
    int q; cin >> q;
    while (q--) {
        int r1, c1, r2, c2; cin >> r1 >> c1 >> r2 >> c2;
        cout << sol.sumRegion(r1, c1, r2, c2) << "\n";
    }
    return 0;
}

