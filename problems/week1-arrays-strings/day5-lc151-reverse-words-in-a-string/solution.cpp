// ============================================================================
// LC-151: Reverse Words in a String
// Difficulty: Medium
// Priority: P1
// Week 1 / Day 5
// ----------------------------------------------------------------------------
// Given an input string s, reverse the order of the words. A word is a
// sequence of non-space characters. Return a string of the words in reverse
// order separated by a single space, with no leading/trailing spaces.
//
// Constraints:
//   - 1 <= s.length <= 1e4
//   - s contains printable ASCII letters, digits, spaces.
//
// Goal: O(n) time, O(1) extra (in-place).
//
// Local I/O format (for test.in):
//   Line 1: the whole string s (may have leading/trailing/multiple spaces)
//   Print the reversed, single-space-joined words.
// Expected output for test.in: blue is sky the
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    string reverseWords(string s) {
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
    getline(cin, s);
    cout << sol.reverseWords(s) << "\n";
    return 0;
}

