// ============================================================================
// LC-31: Next Permutation
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 7
// ----------------------------------------------------------------------------
// Rearrange nums into the next lexicographically greater permutation in-place.
// If no greater permutation exists, rearrange into the lowest possible order.
//
// Constraints:
//   - 1 <= nums.length <= 100
//   - 0 <= nums[i] <= 100
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers
//   Print the next permutation (space-separated).
// Expected output for test.in: 1 3 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    void nextPermutation(vector<int>& nums) {
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
    sol.nextPermutation(a);
    for (int i = 0; i < n; ++i) cout << a[i] << " \n"[i + 1 == n];
    return 0;
}

