// ============================================================================
// LC-153: Find Minimum in Rotated Sorted Array
// Difficulty: Medium
// Priority: P1
// Week 4 / Day 22
// ----------------------------------------------------------------------------
// Given a rotated sorted array of distinct values, return the minimum element.
// O(log n) runtime.
//
// Constraints:
//   - n == nums.length
//   - 1 <= n <= 5000
//   - -5000 <= nums[i] <= 5000
//   - all values unique
//
// Goal: O(log n) time.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the minimum.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int findMin(vector<int>& nums) {
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
    cout << sol.findMin(a) << "\n";
    return 0;
}

