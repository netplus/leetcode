// ============================================================================
// LC-5: Longest Palindromic Substring
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 5
// ----------------------------------------------------------------------------
// Given a string s, return the longest palindromic substring in s.
//
// Constraints:
//   - 1 <= s.length <= 1000
//   - s consists of digits and English letters.
//
// Goal: O(n^2) (expand) or O(n) (Manacher).
//
// Local I/O format (for test.in):
//   Line 1: the string s
//   Print the longest palindromic substring.
// Expected output for test.in: bab
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    string longestPalindrome(string s) {
        // Your implementation here.
        return "";
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    if (!(cin >> s)) return 0;
    cout << sol.longestPalindrome(s) << "\n";
    return 0;
}

