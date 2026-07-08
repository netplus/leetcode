// ============================================================================
// LC-547: Number of Provinces
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 17
// ----------------------------------------------------------------------------
// Given an n x n matrix isConnected where isConnected[i][j] = 1 if city i and j
// are directly connected, a province is a group of directly/indirectly connected
// cities. Return the total number of provinces.
//
// Constraints:
//   - 1 <= n <= 200
//   - n == isConnected.length == isConnected[i].length
//   - isConnected[i][j] in {0,1}, symmetric, isConnected[i][i]==1
//
// Goal: O(n^2) time, O(n) space (union-find).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Next n lines: n space-separated integers per row
//   Print the number of provinces.
// Expected output for test.in: 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int findCircleNum(vector<vector<int>>& isConnected) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.findCircleNum(g) << "\n";
    return 0;
}

