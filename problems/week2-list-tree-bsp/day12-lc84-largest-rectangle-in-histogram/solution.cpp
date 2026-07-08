// ============================================================================
// LC-84: Largest Rectangle in Histogram
// Difficulty: Hard
// Priority: P1
// Week 2 / Day 12
// ----------------------------------------------------------------------------
// Given an array of integers heights representing the bar heights of a
// histogram where each bar is 1 wide, return the area of the largest rectangle
// in the histogram.
//
// Constraints:
//   - 1 <= heights.length <= 1e5
//   - 0 <= heights[i] <= 1e4
//
// Goal: O(n) time (monotonic stack).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the largest rectangle area.
// Expected output for test.in: 10
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int largestRectangleArea(vector<int>& heights) {
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
    cout << sol.largestRectangleArea(a) << "\n";
    return 0;
}

