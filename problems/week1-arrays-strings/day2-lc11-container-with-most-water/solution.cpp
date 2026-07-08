// ============================================================================
// LC-11: Container With Most Water
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 2
// ----------------------------------------------------------------------------
// You are given an array of non-negative integers height where each represents
// a vertical line. Find two lines that with the x-axis form a container holding
// the most water. Return the maximum area.
//
// Constraints:
//   - n == height.length
//   - 2 <= n <= 1e5
//   - 0 <= height[i] <= 1e4
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum area.
// Expected output for test.in: 49
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int maxArea(vector<int>& height) {
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
    cout << sol.maxArea(a) << "\n";
    return 0;
}

