// ============================================================================
// LC-139: Word Break
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 21
// ----------------------------------------------------------------------------
// Given a string s and a dictionary of strings wordDict, return true if s can be
// segmented into a space-separated sequence of one or more dictionary words.
//
// Constraints:
//   - 1 <= s.length <= 300
//   - 1 <= wordDict.length <= 1000
//   - 1 <= wordDict[i].length <= 20
//   - s and words consist of lowercase English letters
//
// Goal: O(n^2) or O(n*L) time (DP).
//
// Local I/O format (for test.in):
//   Line 1: s
//   Line 2: L (number of words)
//   Line 3: L space-separated words
//   Print 1 if s can be segmented, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool wordBreak(string s, vector<string>& wordDict) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string s; if (!(cin >> s)) return 0;
    int L; cin >> L;
    vector<string> w(L);
    for (int i = 0; i < L; ++i) cin >> w[i];
    cout << (sol.wordBreak(s, w) ? 1 : 0) << "\n";
    return 0;
}

