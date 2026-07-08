// ============================================================================
// LC-253: Meeting Rooms II
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 26
// ----------------------------------------------------------------------------
// Given an array of meeting time intervals [start, end), find the minimum number
// of conference rooms required.
//
// Constraints:
//   - 0 <= intervals.length <= 1e4
//   - intervals[i].length == 2
//   - 0 <= start < end <= 1e6
//
// Goal: O(m log m) time (sweep line / heap).
//
// Local I/O format (for test.in):
//   Line 1: m (number of intervals)
//   Next m lines: start end
//   Print the minimum number of rooms.
// Expected output for test.in: 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int minMeetingRooms(vector<vector<int>>& intervals) {
        // Your implementation here.
        return 0;
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
    cout << sol.minMeetingRooms(iv) << "\n";
    return 0;
}

