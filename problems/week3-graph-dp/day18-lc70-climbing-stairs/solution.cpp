// ============================================================================
// LC-70: Climbing Stairs
// Difficulty: Easy
// Priority: P0
// Week 3 / Day 18
// ----------------------------------------------------------------------------
// You are climbing a staircase taking 1 or 2 steps at a time. Return the number
// of distinct ways to reach the top (step n).
//
// Constraints:
//   - 1 <= n <= 45
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Print the number of ways.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int climbStairs(int n) {
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
    cout << sol.climbStairs(n) << "\n";
    return 0;
}

