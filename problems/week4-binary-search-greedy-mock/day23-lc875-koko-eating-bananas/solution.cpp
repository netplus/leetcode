// ============================================================================
// LC-875: Koko Eating Bananas
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 23
// ----------------------------------------------------------------------------
// Koko loves to eat bananas. piles[i] is the bananas in pile i, h is the hours.
// Koko eats at speed k bananas/hour: each hour she picks one pile and eats up to k
// from it. Return the minimum integer k such that she can eat all piles within h
// hours.
//
// Constraints:
//   - 1 <= piles.length <= 1e4
//   - piles.length <= h <= 1e9
//   - 1 <= piles[i] <= 1e9
//
// Goal: O(n log max(piles)).
//
// Local I/O format (for test.in):
//   Line 1: n h
//   Line 2: n space-separated integers
//   Print the minimum eating speed.
// Expected output for test.in: 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int minEatingSpeed(vector<int>& piles, int h) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.minEatingSpeed(a, k) << "\n";
    return 0;
}

