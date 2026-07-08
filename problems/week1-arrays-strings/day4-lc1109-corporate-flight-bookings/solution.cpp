// ============================================================================
// LC-1109: Corporate Flight Bookings
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 4
// ----------------------------------------------------------------------------
// There are n flights labeled 1..n. bookings[i] = [first, last, seats] books
// that many seats on each flight from first to last inclusive. Return an array
// of length n where answer[i] is the total seats booked on flight i+1.
//
// Constraints:
//   - 1 <= n <= 2e4
//   - 1 <= bookings.length <= 2e4
//   - bookings[i].length == 3
//   - 1 <= first <= last <= n
//   - 1 <= seats <= 1e4
//
// Goal: O(n + m) with difference array.
//
// Local I/O format (for test.in):
//   Line 1: n (flights) m (bookings)
//   Next m lines: first last seats
//   Print n space-separated integers (seats per flight, 1-indexed).
// Expected output for test.in: 10 25 33 32 0 0 0 0 0 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> corpFlightBookings(vector<vector<int>>& bookings, int n) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<vector<int>> bk(m, vector<int>(3));
    for (int i = 0; i < m; ++i) cin >> bk[i][0] >> bk[i][1] >> bk[i][2];
    auto ans = sol.corpFlightBookings(bk, n);
    for (int i = 0; i < n; ++i) cout << ans[i] << " \n"[i + 1 == n];
    return 0;
}

