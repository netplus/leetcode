// ============================================================================
// LC-39: Combination Sum
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 13
// ----------------------------------------------------------------------------
// Given an array of distinct integers candidates and a target integer target,
// return a list of all unique combinations of candidates where the chosen numbers
// sum to target. Candidates may be chosen an unlimited number of times.
//
// Constraints:
//   - 1 <= candidates.length <= 30
//   - 2 <= candidate[i] <= 40
//   - all candidates distinct
//   - 1 <= target <= 40
//
// Goal: O(2^(target/min)) time.
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated candidates
//   Print each combination on its own line, space-separated.
// Expected output for test.in: 2 2 3 | 7
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        // Your implementation here.
        return {};
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
    auto res = sol.combinationSum(a, target);
    for (auto& c : res) {
        for (size_t i = 0; i < c.size(); ++i) cout << c[i] << " \n"[i + 1 == c.size()];
    }
    return 0;
}

