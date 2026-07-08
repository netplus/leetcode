// ============================================================================
// LC-42: Trapping Rain Water
// Difficulty: Hard
// Priority: P0
// Week 4 / Day 26
// ----------------------------------------------------------------------------
// Given n non-negative integers representing an elevation map where the width of
// each bar is 1, compute how much water it can trap after raining.
//
// Constraints:
//   - n == height.length
//   - 1 <= n <= 2e4
//   - 0 <= height[i] <= 1e5
//
// Goal: O(n) time, O(1) space (two pointers).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the total units of trapped water.
// Expected output for test.in: 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int trap(vector<int>& height) {
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
    cout << sol.trap(a) << "\n";
    return 0;
}

