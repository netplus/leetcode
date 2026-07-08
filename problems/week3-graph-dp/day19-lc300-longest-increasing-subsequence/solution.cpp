// ============================================================================
// LC-300: Longest Increasing Subsequence
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 19
// ----------------------------------------------------------------------------
// Given an integer array nums, return the length of the longest strictly
// increasing subsequence.
//
// Constraints:
//   - 1 <= nums.length <= 2500
//   - -1e4 <= nums[i] <= 1e4
//
// Goal: O(n log n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the LIS length.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int lengthOfLIS(vector<int>& nums) {
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
    cout << sol.lengthOfLIS(a) << "\n";
    return 0;
}

