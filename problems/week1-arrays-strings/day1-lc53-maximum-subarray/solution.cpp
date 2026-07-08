// ============================================================================
// LC-53: Maximum Subarray
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 1
// ----------------------------------------------------------------------------
// Given an integer array nums, find the subarray with the largest sum, and
// return its sum.
//
// Constraints:
//   - 1 <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4
//
// Goal: O(n) time, O(1) space (Kadane).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum subarray sum.
// Expected output for test.in: 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int maxSubArray(vector<int>& nums) {
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
    cout << sol.maxSubArray(a) << "\n";
    return 0;
}

