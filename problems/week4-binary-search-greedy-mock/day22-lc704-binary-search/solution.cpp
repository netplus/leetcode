// ============================================================================
// LC-704: Binary Search
// Difficulty: Easy
// Priority: P0
// Week 4 / Day 22
// ----------------------------------------------------------------------------
// Given an array of integers nums sorted in ascending order and an integer target,
// write a function to search target in nums. Return its index, or -1 if not present.
// O(log n) runtime.
//
// Constraints:
//   - 1 <= nums.length <= 1e4
//   - -9999 <= nums[i], target <= 9999
//   - all integers unique, nums sorted ascending
//
// Goal: O(log n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the index, or -1.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int search(vector<int>& nums, int target) {
        // Your implementation here.
        return -1;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.search(a, target) << "\n";
    return 0;
}

