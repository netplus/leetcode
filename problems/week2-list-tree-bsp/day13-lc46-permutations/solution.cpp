// ============================================================================
// LC-46: Permutations
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 13
// ----------------------------------------------------------------------------
// Given an array nums of distinct integers, return all possible permutations.
// The answer may be returned in any order.
//
// Constraints:
//   - 1 <= nums.length <= 6
//   - -10 <= nums[i] <= 10
//   - all integers are unique
//
// Goal: O(n! * n) time.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print each permutation on its own line, space-separated.
// Expected output for test.in: 1 2 3 | 1 3 2 | 2 1 3 | 2 3 1 | 3 1 2 | 3 2 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> permute(vector<int>& nums) {
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
    auto res = sol.permute(a);
    for (auto& p : res) {
        for (size_t i = 0; i < p.size(); ++i) cout << p[i] << " \n"[i + 1 == p.size()];
    }
    return 0;
}

