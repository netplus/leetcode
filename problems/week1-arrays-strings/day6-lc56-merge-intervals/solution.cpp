// ============================================================================
// LC-56: Merge Intervals
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 6
// ----------------------------------------------------------------------------
// Given an array of intervals where intervals[i] = [start, end], merge all
// overlapping intervals, and return an array of the non-overlapping intervals
// covering all input intervals.
//
// Constraints:
//   - 1 <= intervals.length <= 1e4
//   - intervals[i].length == 2
//   - 0 <= start <= end <= 1e4
//
// Goal: O(m log m) time.
//
// Local I/O format (for test.in):
//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Print merged intervals one per line (start end), sorted by start.
// Expected output for test.in: 1 6 | 8 10 | 15 18
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> iv(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> iv[i][0] >> iv[i][1];
    auto res = sol.merge(iv);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\n";
    return 0;
}

