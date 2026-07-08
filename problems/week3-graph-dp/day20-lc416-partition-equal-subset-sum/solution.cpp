// ============================================================================
// LC-416: Partition Equal Subset Sum
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 20
// ----------------------------------------------------------------------------
// Given a non-empty array nums of positive integers, determine if the array can
// be partitioned into two subsets such that the sum of elements in both subsets is
// equal.
//
// Constraints:
//   - 1 <= nums.length <= 200
//   - 1 <= nums[i] <= 100
//
// Goal: O(n*sum) time, O(sum) space (0/1 knapsack).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print 1 if partitionable into equal sums, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool canPartition(vector<int>& nums) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.canPartition(a) ? 1 : 0) << "\n";
    return 0;
}

