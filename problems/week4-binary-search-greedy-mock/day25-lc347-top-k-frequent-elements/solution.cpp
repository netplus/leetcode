// ============================================================================
// LC-347: Top K Frequent Elements
// Difficulty: Medium
// Priority: P0
// Week 4 / Day 25
// ----------------------------------------------------------------------------
// Given an integer array nums and an integer k, return the k most frequent elements.
// The answer may be returned in any order.
//
// Constraints:
//   - 1 <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4
//   - k is in [1, number of unique elements]
//
// Goal: O(n log k) time.
//
// Local I/O format (for test.in):
//   Line 1: n k
//   Line 2: n space-separated integers
//   Print the k most frequent elements space-separated (any order).
// Expected output for test.in: 1 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> topKFrequent(vector<int>& nums, int k) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.topKFrequent(a, k);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

