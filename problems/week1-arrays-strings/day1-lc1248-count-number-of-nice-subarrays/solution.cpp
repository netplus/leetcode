// ============================================================================
// LC-1248: Count Number of Nice Subarrays
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 1
// ----------------------------------------------------------------------------
// Given an array nums of integers and an integer k, return the number of
// "nice" subarrays (continuous subarrays with exactly k odd numbers).
//
// Constraints:
//   - 1 <= nums.length <= 5e4
//   - 1 <= k <= nums.length
//   - 1 <= nums[i] <= 1e5
//
// Goal: O(n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the number of nice subarrays.
// Expected output for test.in: 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int numberOfSubarrays(vector<int>& nums, int k) {
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
    cout << sol.numberOfSubarrays(a, k) << "\n";
    return 0;
}

