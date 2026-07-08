// ============================================================================
// LC-33: Search in Rotated Sorted Array
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 22
// ----------------------------------------------------------------------------
// Given a rotated sorted array nums of distinct values and a target, return the
// index of target, or -1. O(log n) runtime.
//
// Constraints:
//   - 1 <= nums.length <= 5000
//   - -1e4 <= nums[i] <= 1e4
//   - all values unique, nums rotated at some pivot
//
// Goal: O(log n) time.
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the index, or -1.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int search(vector<int>& nums, int target) {
        // Your implementation here.
        return -1;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.search(a, target) << "\n";
    return 0;
}

