// ============================================================================
// LC-252: Meeting Rooms
// Difficulty: Easy
// Priority: P1
// Week 1 / Day 6
// ----------------------------------------------------------------------------
// Given an array of meeting time intervals where intervals[i] = [start, end],
// determine if a person could attend all meetings (no overlaps).
//
// Constraints:
//   - 0 <= intervals.length <= 1e4
//   - intervals[i].length == 2
//   - 0 <= start < end <= 1e6
//
// Goal: O(m log m) time.
//
// Local I/O format (for test.in):
//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Print 1 if can attend all (no overlap), else 0.
// Expected output for test.in: 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        // Your implementation here.
        return false;
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
    cout << (sol.canAttendMeetings(iv) ? 1 : 0) << "\n";
    return 0;
}

