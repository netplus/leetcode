// ============================================================================
// LC-322: Coin Change
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 20
// ----------------------------------------------------------------------------
// Given an integer array coins representing denominations and an integer amount,
// return the fewest number of coins needed to make up that amount. If impossible,
// return -1. You may use each coin unlimited times.
//
// Constraints:
//   - 1 <= coins.length <= 12
//   - 1 <= coins[i] <= 2^31 - 1
//   - 0 <= amount <= 1e4
//
// Goal: O(n*amount) time (complete knapsack).
//
// Local I/O format (for test.in):
//   Line 1: n amount
//   Line 2: n space-separated coin denominations
//   Print the fewest number of coins, or -1.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int coinChange(vector<int>& coins, int amount) {
        // Your implementation here.
        return -1;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, amount; if (!(cin >> n >> amount)) return 0;
    vector<int> c(n);
    for (int i = 0; i < n; ++i) cin >> c[i];
    cout << sol.coinChange(c, amount) << "\n";
    return 0;
}

