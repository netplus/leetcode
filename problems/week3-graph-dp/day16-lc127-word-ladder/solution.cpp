// ============================================================================
// LC-127: Word Ladder
// Difficulty: Hard
// Priority: P0
// Week 3 / Day 16
// ----------------------------------------------------------------------------
// Given two words beginWord and endWord and a dictionary wordList, return the
// number of words in the shortest transformation sequence from beginWord to
// endWord where each step changes exactly one letter and each intermediate word
// must be in wordList. Return 0 if no such sequence.
//
// Constraints:
//   - 1 <= beginWord.length <= 10
//   - endWord.length == beginWord.length
//   - 1 <= wordList.length <= 5000
//   - all words unique, lowercase letters
//
// Goal: O(L^2 * N) with BFS.
//
// Local I/O format (for test.in):
//   Line 1: beginWord endWord
//   Line 2: L (number of words in wordList)
//   Line 3: L space-separated words
//   Print the length of the shortest transformation sequence (number of words).
// Expected output for test.in: 5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string b, e; if (!(cin >> b >> e)) return 0;
    int L; cin >> L;
    vector<string> wl(L);
    for (int i = 0; i < L; ++i) cin >> wl[i];
    cout << sol.ladderLength(b, e, wl) << "\n";
    return 0;
}

