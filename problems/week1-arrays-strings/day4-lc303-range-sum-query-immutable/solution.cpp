// ============================================================================
// LC-303: Range Sum Query - Immutable
// Difficulty: Easy
// Priority: P0
// Week 1 / Day 4
// ----------------------------------------------------------------------------
// Given an integer array nums, handle multiple queries: sumRange(left, right)
// returns the sum of elements nums[left..right] inclusive.
// (LC class NumArray; here modeled as Solution with build() + sumRange().)
//
// Constraints:
//   - 1 <= nums.length <= 1e4
//   - -1e5 <= nums[i] <= 1e5
//   - 0 <= left <= right < nums.length
//   - at most 1e4 calls
//
// Goal: O(n) build, O(1) query.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Line 3: q (number of queries)
//   Next q lines: left right
//   Print each sum on its own line.
// Expected output for test.in: 1 -1 -3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int sumRange(int left, int right) {
        // Your implementation here.
        return 0;
    }
    void build(vector<int>& nums) {
        // TODO: precompute prefix sums into a member variable.
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
    sol.build(a);
    int q; cin >> q;
    while (q--) {
        int l, r; cin >> l >> r;
        cout << sol.sumRange(l, r) << "\n";
    }
    return 0;
}

