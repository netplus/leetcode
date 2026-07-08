// ============================================================================
// LC-167: Two Sum II - Input Array Is Sorted
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 2
// ----------------------------------------------------------------------------
// Given a 1-indexed array of integers numbers that is already sorted in
// non-decreasing order, find two numbers that add up to a specific target.
// Return the indices (1-indexed), index1 < index2. Exactly one solution.
//
// Constraints:
//   - 2 <= numbers.length <= 3e4
//   - numbers sorted non-decreasing
//   - exactly one valid answer exists
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print two 1-indexed indices (space-separated, ascending).
// Expected output for test.in: 1 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> twoSum(vector<int>& numbers, int target) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target;
    if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.twoSum(a, target);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

