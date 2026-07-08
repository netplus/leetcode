// ============================================================================
// LC-57: Insert Interval
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 6
// ----------------------------------------------------------------------------
// Given a set of non-overlapping sorted intervals and a new interval, insert
// the new interval, merging as needed. Return the resulting intervals.
//
// Constraints:
//   - 0 <= intervals.length <= 1e4
//   - intervals sorted ascending by start, non-overlapping
//   - 0 <= start <= end <= 1e5
//
// Goal: O(n) time.
//
// Local I/O format (for test.in):
//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Last line: start end (the new interval)
//   Print merged intervals one per line (start end).
// Expected output for test.in: 1 5 | 6 8
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
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
    vector<int> ni(2); cin >> ni[0] >> ni[1];
    auto res = sol.insert(iv, ni);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\n";
    return 0;
}

