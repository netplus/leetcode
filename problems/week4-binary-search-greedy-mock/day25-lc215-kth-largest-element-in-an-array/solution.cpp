// ============================================================================
// LC-215: Kth Largest Element in an Array
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 25
// ----------------------------------------------------------------------------
// Given an integer array nums and an integer k, return the kth largest element in
// the array (not the kth distinct element).
//
// Constraints:
//   - 1 <= k <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4
//
// Goal: O(n) avg (quickselect) or O(n log k) (heap).
//
// Local I/O format (for test.in):
//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the kth largest element.
// Expected output for test.in: 5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int findKthLargest(vector<int>& nums, int k) {
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
    cout << sol.findKthLargest(a, k) << "\n";
    return 0;
}

