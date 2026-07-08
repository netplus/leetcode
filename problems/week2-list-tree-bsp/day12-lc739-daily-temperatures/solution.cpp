// ============================================================================
// LC-739: Daily Temperatures
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 12
// ----------------------------------------------------------------------------
// Given an array of integers temperatures representing daily temperatures,
// return an array answer such that answer[i] is the number of days after the
// ith day until a warmer temperature. If none, answer[i] == 0.
//
// Constraints:
//   - 1 <= temperatures.length <= 1e5
//   - 30 <= temperatures[i] <= 100
//
// Goal: O(n) time (monotonic stack).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print n space-separated integers (days to wait).
// Expected output for test.in: 1 1 4 2 1 1 0 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        // Your implementation here.
        return {};
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
    auto ans = sol.dailyTemperatures(a);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

