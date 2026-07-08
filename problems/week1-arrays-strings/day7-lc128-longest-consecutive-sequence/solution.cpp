// ============================================================================
// LC-128: Longest Consecutive Sequence
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 7
// ----------------------------------------------------------------------------
// Given an unsorted array of integers nums, return the length of the longest
// consecutive elements sequence. O(n) time.
//
// Constraints:
//   - 0 <= nums.length <= 1e5
//   - -1e9 <= nums[i] <= 1e9
//
// Goal: O(n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the longest consecutive sequence length.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int longestConsecutive(vector<int>& nums) {
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
    cout << sol.longestConsecutive(a) << "\n";
    return 0;
}

