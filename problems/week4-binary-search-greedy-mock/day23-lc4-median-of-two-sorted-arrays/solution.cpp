// ============================================================================
// LC-4: Median of Two Sorted Arrays
// Difficulty: Hard
// Priority: P1
// Week 4 / Day 23
// ----------------------------------------------------------------------------
// Given two sorted arrays nums1 and nums2 of sizes m and n, return the median of
// the two sorted arrays. The overall runtime must be O(log(m+n)).
//
// Constraints:
//   - 0 <= m, n <= 1000
//   - 1 <= m + n
//   - -1e6 <= nums1[i], nums2[i] <= 1e6
//
// Goal: O(log(min(m,n))) time.
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Line 2: m space-separated integers (nums1)
//   Line 3: n space-separated integers (nums2)
//   Print the median (as a float, e.g. 2.0 or 2.5).
// Expected output for test.in: 2.5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        // Your implementation here.
        return 0.0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<int> a(m), b(n);
    for (int i = 0; i < m; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];
    cout << sol.findMedianSortedArrays(a, b) << "\n";
    return 0;
}

