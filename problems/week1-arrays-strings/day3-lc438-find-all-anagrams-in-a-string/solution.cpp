// ============================================================================
// LC-438: Find All Anagrams in a String
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 3
// ----------------------------------------------------------------------------
// Given two strings s and p, return an array of all start indices of p's
// anagrams in s. The answer may be returned in any order.
//
// Constraints:
//   - 1 <= s.length, p.length <= 3e4
//   - s and p consist of lowercase English letters.
//
// Goal: O(|s|) time.
//
// Local I/O format (for test.in):
//   Line 1: s
//   Line 2: p
//   Print the start indices (space-separated, ascending).
// Expected output for test.in: 0 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<int> findAnagrams(string s, string p) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s, p; if (!(cin >> s >> p)) return 0;
    auto ans = sol.findAnagrams(s, p);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

