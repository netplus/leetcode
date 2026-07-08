// ============================================================================
// LC-179: Largest Number
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 6
// ----------------------------------------------------------------------------
// Given a list of non-negative integers nums, arrange them such that they
// form the largest number and return it as a string.
//
// Constraints:
//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 1e9
//
// Goal: O(n log n * L).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the largest number string.
// Expected output for test.in: 210
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    string largestNumber(vector<int>& nums) {
        // Your implementation here.
        return "";
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
    cout << sol.largestNumber(a) << "\n";
    return 0;
}

