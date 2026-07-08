// ============================================================================
// LC-1143: Longest Common Subsequence
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 19
// ----------------------------------------------------------------------------
// Given two strings text1 and text2, return the length of their longest common
// subsequence. If no common subsequence, return 0.
//
// Constraints:
//   - 1 <= text1.length, text2.length <= 1000
//   - consist of lowercase English letters
//
// Goal: O(m*n) time, O(min(m,n)) space.
//
// Local I/O format (for test.in):
//   Line 1: text1
//   Line 2: text2
//   Print the LCS length.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int longestCommonSubsequence(string text1, string text2) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string a, b; if (!(cin >> a >> b)) return 0;
    cout << sol.longestCommonSubsequence(a, b) << "\n";
    return 0;
}

