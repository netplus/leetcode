// ============================================================================
// LC-62: Unique Paths
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 19
// ----------------------------------------------------------------------------
// A robot at the top-left corner of an m x n grid moves only right or down.
// Return the number of possible unique paths to the bottom-right corner.
//
// Constraints:
//   - 1 <= m, n <= 100
//   - answer fits in a 32-bit int (test inputs guaranteed)
//
// Goal: O(m*n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Print the number of unique paths.
// Expected output for test.in: 28
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int uniquePaths(int m, int n) {
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
    cout << sol.uniquePaths(m, n) << "\n";
    return 0;
}

