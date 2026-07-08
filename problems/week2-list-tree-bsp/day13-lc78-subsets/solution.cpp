// ============================================================================
// LC-78: Subsets
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 13
// ----------------------------------------------------------------------------
// Given an integer array nums of unique elements, return all possible subsets
// (the power set). The solution set must not contain duplicate subsets.
//
// Constraints:
//   - 1 <= nums.length <= 10
//   - -10 <= nums[i] <= 10
//   - all integers unique
//
// Goal: O(n * 2^n).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print each subset on its own line, space-separated (empty subset = blank line).
// Expected output for test.in: [] | [1] | [2] | [1 2] | [3] | [1 3] | [2 3] | [1 2 3]
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> subsets(vector<int>& nums) {
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
    auto res = sol.subsets(a);
    for (auto& s : res) {
        for (size_t i = 0; i < s.size(); ++i) cout << s[i] << " \n"[i + 1 == s.size()];
        if (s.empty()) cout << "\n";
    }
    return 0;
}

