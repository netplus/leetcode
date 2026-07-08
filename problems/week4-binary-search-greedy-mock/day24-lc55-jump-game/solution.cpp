// ============================================================================
// LC-55: Jump Game
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 24
// ----------------------------------------------------------------------------
// You are at index 0 of nums; nums[i] is the maximum jump length from i. Return
// true if you can reach the last index.
//
// Constraints:
//   - 1 <= nums.length <= 1e4
//   - 0 <= nums[i] <= 1e5
//
// Goal: O(n) time, O(1) space (greedy).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print 1 if reachable, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool canJump(vector<int>& nums) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.canJump(a) ? 1 : 0) << "\n";
    return 0;
}

