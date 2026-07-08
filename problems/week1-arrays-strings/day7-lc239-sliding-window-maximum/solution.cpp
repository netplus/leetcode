// ============================================================================
// LC-239: Sliding Window Maximum
// Difficulty: Hard
// Priority: P0
// Week 1 / Day 7
// ----------------------------------------------------------------------------
// Return the max value in each sliding window of size k moving from left to
// right across nums. Return an array of the maxes.
//
// Constraints:
//   - 1 <= nums.length <= 1e5
//   - -1e4 <= nums[i] <= 1e4
//   - 1 <= k <= nums.length
//
// Goal: O(n) time (monotonic deque).
//
// Local I/O format (for test.in):
//   Line 1: n k
//   Line 2: n space-separated integers
//   Print (n-k+1) space-separated maxes.
// Expected output for test.in: 3 3 5 5 6 7
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
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
    auto ans = sol.maxSlidingWindow(a, k);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

