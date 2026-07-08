// ============================================================================
// LC-152: Maximum Product Subarray
// Difficulty: Medium
// Priority: P1
// Week 3 / Day 21
// ----------------------------------------------------------------------------
// Given an integer array nums, find a contiguous non-empty subarray with the
// largest product and return the product.
//
// Constraints:
//   - 1 <= nums.length <= 2e4
//   - -10 <= nums[i] <= 10
//   - product fits in a 32-bit int
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum product.
// Expected output for test.in: 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int maxProduct(vector<int>& nums) {
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
    cout << sol.maxProduct(a) << "\n";
    return 0;
}

