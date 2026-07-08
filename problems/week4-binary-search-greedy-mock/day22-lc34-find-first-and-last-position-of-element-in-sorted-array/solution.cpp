// ============================================================================
// LC-34: Find First and Last Position of Element in Sorted Array
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 22
// ----------------------------------------------------------------------------
// Given a sorted array nums (non-decreasing) and a target, find the starting
// and ending positions of target. Return [-1, -1] if not found. O(log n).
//
// Constraints:
//   - 0 <= nums.length <= 1e5
//   - -1e9 <= nums[i] <= 1e9
//   - nums sorted ascending
//
// Goal: O(log n) time (two binary searches).
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print "left right" (the bounds), or "-1 -1".
// Expected output for test.in: 3 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> searchRange(vector<int>& nums, int target) {
        // Your implementation here.
        return {-1, -1};
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
    auto ans = sol.searchRange(a, target);
    cout << ans[0] << " " << ans[1] << "\n";
    return 0;
}

