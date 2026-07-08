// ============================================================================
// LC-1011: Capacity To Ship Packages Within D Days
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 23
// ----------------------------------------------------------------------------
// weights[i] must be shipped in order within D days; the ship has a fixed capacity.
// Return the least capacity that ships all packages within D days.
//
// Constraints:
//   - 1 <= weights.length <= 5e4
//   - 1 <= weights[i] <= 500
//   - 1 <= D <= weights.length
//
// Goal: O(n log S).
//
// Local I/O format (for test.in):
//   Line 1: n D
//   Line 2: n space-separated integers
//   Print the least ship capacity.
// Expected output for test.in: 15
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int shipWithinDays(vector<int>& weights, int days) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.shipWithinDays(a, k) << "\n";
    return 0;
}

