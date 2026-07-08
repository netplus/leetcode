// ============================================================================
// LC-45: Jump Game II
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 24
// ----------------------------------------------------------------------------
// Like Jump Game, but return the minimum number of jumps to reach the last index.
// It is guaranteed reachable.
//
// Constraints:
//   - 1 <= nums.length <= 1e4
//   - 0 <= nums[i] <= 1000
//   - reachable to last index
//
// Goal: O(n) time, O(1) space (greedy).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the minimum number of jumps.
// Expected output for test.in: 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int jump(vector<int>& nums) {
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
    cout << sol.jump(a) << "\n";
    return 0;
}

