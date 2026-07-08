// ============================================================================
// LC-41: First Missing Positive
// Difficulty: Hard
// Priority: P0
// Week 1 / Day 7
// ----------------------------------------------------------------------------
// Given an unsorted integer array nums, return the smallest positive integer
// not present in nums. O(n) time, O(1) auxiliary space.
//
// Constraints:
//   - 1 <= nums.length <= 1e5
//   - -2^31 <= nums[i] <= 2^31 - 1
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the smallest missing positive integer.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int firstMissingPositive(vector<int>& nums) {
        // Your implementation here.
        return 1;
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
    cout << sol.firstMissingPositive(a) << "\n";
    return 0;
}

