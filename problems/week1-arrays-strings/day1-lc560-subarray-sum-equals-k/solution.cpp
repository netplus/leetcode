// ============================================================================
// LC-560: Subarray Sum Equals K
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 1
// ----------------------------------------------------------------------------
// Given an array of integers nums and an integer k, return the total number of
// continuous subarrays whose sum equals k.
//
// Constraints:
//   - 1 <= nums.length <= 2e4
//   - -1000 <= nums[i] <= 1000
//   - -1e7 <= k <= 1e7
//
// Goal: O(n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the count of subarrays summing to k.
// Expected output for test.in: 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int subarraySum(vector<int>& nums, int k) {
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
    cout << sol.subarraySum(a, k) << "\n";
    return 0;
}

