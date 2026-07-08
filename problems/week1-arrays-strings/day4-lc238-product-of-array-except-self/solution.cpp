// ============================================================================
// LC-238: Product of Array Except Self
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 4
// ----------------------------------------------------------------------------
// Given an integer array nums, return an array answer such that answer[i]
// equals the product of all elements of nums except nums[i]. O(n), no division.
//
// Constraints:
//   - 2 <= nums.length <= 1e5
//   - -30 <= nums[i] <= 30
//   - product of any prefix/suffix fits in a 32-bit int
//
// Goal: O(n) time, O(1) extra space (output excluded).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print n space-separated integers (the products).
// Expected output for test.in: 24 12 8 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> productExceptSelf(vector<int>& nums) {
        // Your implementation here.
        return {};
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
    auto ans = sol.productExceptSelf(a);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

