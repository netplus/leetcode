// ============================================================================
// LC-410: Split Array Largest Sum
// Difficulty: Hard
// Priority: P0
// Week 4 / Day 23
// ----------------------------------------------------------------------------
// Given an array nums of non-negative integers and an integer k, split nums into
// k non-empty contiguous subarrays such that the largest sum among the subarrays is
// minimized. Return the minimized largest sum.
//
// Constraints:
//   - 1 <= nums.length <= 1000
//   - 0 <= nums[i] <= 1e6
//   - 1 <= k <= min(50, nums.length)
//
// Goal: O(n log S) where S = sum(nums).
//
// Local I/O format (for test.in):
//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the minimized largest sum.
// Expected output for test.in: 18
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int splitArray(vector<int>& nums, int k) {
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
    cout << sol.splitArray(a, k) << "\n";
    return 0;
}

