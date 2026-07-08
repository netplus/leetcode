// ============================================================================
// LC-198: House Robber
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 18
// ----------------------------------------------------------------------------
// You are a robber along a street where nums[i] is the money in house i. You
// cannot rob adjacent houses. Return the maximum amount you can rob.
//
// Constraints:
//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 400
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum amount.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int rob(vector<int>& nums) {
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
    cout << sol.rob(a) << "\n";
    return 0;
}

