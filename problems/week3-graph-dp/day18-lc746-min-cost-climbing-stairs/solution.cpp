// ============================================================================
// LC-746: Min Cost Climbing Stairs
// Difficulty: Easy
// Priority: P1
// Week 3 / Day 18
// ----------------------------------------------------------------------------
// Given an array cost where cost[i] is the cost of step i, you can start at
// index 0 or 1, and each step you can climb 1 or 2 steps. Return the minimum cost
// to reach the top (past the last index).
//
// Constraints:
//   - 2 <= cost.length <= 1000
//   - 0 <= cost[i] <= 999
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the minimum cost.
// Expected output for test.in: 15
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int minCostClimbingStairs(vector<int>& cost) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.minCostClimbingStairs(a) << "\n";
    return 0;
}

