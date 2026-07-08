// ============================================================================
// LC-14: Longest Common Prefix
// Difficulty: Easy
// Priority: P0
// Week 1 / Day 5
// ----------------------------------------------------------------------------
// Find the longest common prefix string amongst an array of strings. If none,
// return the empty string.
//
// Constraints:
//   - 1 <= strs.length <= 200
//   - 0 <= strs[i].length <= 200
//   - strs[i] consists of lowercase English letters.
//
// Goal: O(S) where S = total characters.
//
// Local I/O format (for test.in):
//   Line 1: n (number of strings)
//   Next n lines: one string each
//   Print the longest common prefix.
// Expected output for test.in: fl
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    string longestCommonPrefix(vector<string>& strs) {
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
    vector<string> strs(n);
    for (int i = 0; i < n; ++i) cin >> strs[i];
    cout << sol.longestCommonPrefix(strs) << "\n";
    return 0;
}

