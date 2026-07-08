// ============================================================================
// LC-1: Two Sum
// Difficulty: Easy
// Priority: P0
// Week 1 / Day 1
// ----------------------------------------------------------------------------
// Given an array of integers nums and an integer target, return indices of the
// two numbers such that they add up to target.
// Each input has exactly one solution, and you may not use the same element twice.
// You can return the answer in any order.
//
// Constraints:
//   - 2 <= nums.length <= 1e4
//   - -1e9 <= nums[i] <= 1e9
//   - exactly one valid answer exists
//
// Goal: O(n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: n target
//   Line 2: n space-separated integers
//   Print the two indices (space-separated, ascending).
// Expected output for test.in: 0 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

#include <unordered_map>

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> twoSum(vector<int>& nums, int target) {
        // Your implementation here.

        std::unordered_map<int,int> h;

        for( int i=0;i<nums.size();++i)
        {
            int t = target - nums[i];
            if(h.count(t))
            {
                return {h[t],i};
            }
            h.insert(std::make_pair(nums[i],i));
        }
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target;
    if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.twoSum(a, target);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

