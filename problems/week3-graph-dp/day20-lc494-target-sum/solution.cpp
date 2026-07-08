// ============================================================================
// LC-494: Target Sum
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 20
// ----------------------------------------------------------------------------
// Given an array nums and a target, assign + or - to each element and concatenate
// them. Return the number of different expressions that evaluate to target.
//
// Constraints:
//   - 1 <= nums.length <= 20
//   - 0 <= nums[i] <= 1000
//   - 0 <= sum(nums) <= 1000
//   - -1000 <= target <= 1000
//
// Goal: O(n*sum) time (0/1 knapsack).
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the number of expressions evaluating to target.
// Expected output for test.in: 5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int findTargetSumWays(vector<int>& nums, int target) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.findTargetSumWays(a, k) << "\n";
    return 0;
}

