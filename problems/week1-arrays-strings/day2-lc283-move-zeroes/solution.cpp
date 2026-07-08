// ============================================================================
// LC-283: Move Zeroes
// Difficulty: Easy
// Priority: P1
// Week 1 / Day 2
// ----------------------------------------------------------------------------
// Given an integer array nums, move all 0s to the end while maintaining the
// relative order of the non-zero elements. Do this in-place.
//
// Constraints:
//   - 1 <= nums.length <= 1e4
//   - -2^31 <= nums[i] <= 2^31 - 1
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the array after moving zeroes (space-separated).
// Expected output for test.in: 1 3 12 0 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    void moveZeroes(vector<int>& nums) {
        // Your implementation here.
        return ;
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
    sol.moveZeroes(a);
    for (int i = 0; i < n; ++i) cout << a[i] << " \n"[i + 1 == n];
    return 0;
}

