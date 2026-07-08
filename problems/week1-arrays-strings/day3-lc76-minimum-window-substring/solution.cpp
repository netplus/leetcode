// ============================================================================
// LC-76: Minimum Window Substring
// Difficulty: Hard
// Priority: P0
// Week 1 / Day 3
// ----------------------------------------------------------------------------
// Given two strings s and t, return the minimum window substring of s such
// that every character in t (including duplicates) is in the window. If none,
// return the empty string.
//
// Constraints:
//   - 1 <= s.length, t.length <= 1e5
//   - s and t consist of uppercase/lowercase English letters.
//
// Goal: O(|s| + |t|) time.
//
// Local I/O format (for test.in):
//   Line 1: s
//   Line 2: t
//   Print the minimum window substring.
// Expected output for test.in: BANC
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    string minWindow(string s, string t) {
        // Your implementation here.
        return "";
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s, t; if (!(cin >> s >> t)) return 0;
    cout << sol.minWindow(s, t) << "\n";
    return 0;
}

