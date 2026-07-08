// ============================================================================
// LC-20: Valid Parentheses
// Difficulty: Easy
// Priority: P0
// Week 2 / Day 12
// ----------------------------------------------------------------------------
// Given a string s containing just the characters '(', ')', '{', '}', '[' and
// ']', determine if the input string is valid (brackets correctly matched and
// closed in the correct order).
//
// Constraints:
//   - 1 <= s.length <= 1e4
//   - s consists of parentheses-type characters only
//
// Goal: O(n) time, O(n) space.
//
// Local I/O format (for test.in):
//   Line 1: the string s
//   Print 1 if valid, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool isValid(string s) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s;
    if (!(cin >> s)) return 0;
    cout << (sol.isValid(s) ? 1 : 0) << "\n";
    return 0;
}

