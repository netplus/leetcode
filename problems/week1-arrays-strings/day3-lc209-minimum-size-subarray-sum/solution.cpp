// ============================================================================
// LC-209: Minimum Size Subarray Sum
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 3
// ----------------------------------------------------------------------------
// Given an array of positive integers nums and a positive integer target,
// return the minimal length of a contiguous subarray whose sum is >= target.
// If none, return 0.
//
// Constraints:
//   - 1 <= target <= 1e9
//   - 1 <= nums.length <= 1e5
//   - 1 <= nums[i] <= 1e4
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the minimal length.
// Expected output for test.in: 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int minSubArrayLen(int target, vector<int>& nums) {
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
    cout << sol.minSubArrayLen(k, a) << "\n";
    return 0;
}

