// ============================================================================
// LC-312: Burst Balloons
// Difficulty: Hard
// Priority: P1
// Week 3 / Day 20
// ----------------------------------------------------------------------------
// Given n balloons indexed 0..n-1 with nums[i] painted on each, bursting balloon i
// gives nums[left]*nums[i]*nums[right] coins (left/right are adjacent at burst
// time). Return the maximum coins obtainable by bursting all balloons.
//
// Constraints:
//   - n == nums.length
//   - 1 <= n <= 300
//   - 0 <= nums[i] <= 100
//
// Goal: O(n^3) time (interval DP).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the maximum coins.
// Expected output for test.in: 167
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int maxCoins(vector<int>& nums) {
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
    cout << sol.maxCoins(a) << "\n";
    return 0;
}

