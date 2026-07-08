// ============================================================================
// LC-15: 3Sum
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 2
// ----------------------------------------------------------------------------
// Given an integer array nums, return all unique triplets
// [nums[i], nums[j], nums[k]] such that i != j != k and they sum to 0.
// The solution set must not contain duplicate triplets.
//
// Constraints:
//   - 3 <= nums.length <= 3000
//   - -1e5 <= nums[i] <= 1e5
//
// Goal: O(n^2) time.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print each triplet "a b c" on its own line; triplets sorted, no duplicates.
// Expected output for test.in: -1 0 1 | -1 -1 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> threeSum(vector<int>& nums) {
        // Your implementation here.
        return {};
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
    auto res = sol.threeSum(a);
    for (auto& t : res) cout << t[0] << " " << t[1] << " " << t[2] << "\n";
    return 0;
}

