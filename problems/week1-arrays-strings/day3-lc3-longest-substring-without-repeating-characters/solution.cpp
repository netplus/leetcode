// ============================================================================
// LC-3: Longest Substring Without Repeating Characters
// Difficulty: Medium
// Priority: P0
// Week 1 / Day 3
// ----------------------------------------------------------------------------
// Given a string s, find the length of the longest substring without
// repeating characters.
//
// Constraints:
//   - 0 <= s.length <= 5e4
//   - s consists of English letters, digits, symbols and spaces.
//
// Goal: O(n) time, O(min(alphabet,n)) space.
//
// Local I/O format (for test.in):
//   Line 1: the string s
//   Print the length.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int lengthOfLongestSubstring(string s) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    if (!(cin >> s)) return 0;
    cout << sol.lengthOfLongestSubstring(s) << "\n";
    return 0;
}

