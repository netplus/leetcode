// ============================================================================
// LC-79: Word Search
// Difficulty: Medium
// Priority: P0
// Week 3 / Day 21
// ----------------------------------------------------------------------------
// Given an m x n grid of characters and a string word, return true if word exists
// in the grid (constructed from adjacent cells, each cell used once).
//
// Constraints:
//   - m == board.length, n == board[i].length
//   - 1 <= m, n <= 6
//   - 1 <= word.length <= 15
//   - board and word consist of lowercase English letters
//
// Goal: O(m*n*4^L) time (backtracking).
//
// Local I/O format (for test.in):
//   Line 1: m n
//   Next m lines: each row as a string of n chars
//   Line: the word
//   Print 1 if the word exists, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool exist(vector<vector<char>>& board, string word) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<char>> b(m, vector<char>(n));
    for (int i = 0; i < m; ++i) { string row; cin >> row; for (int j = 0; j < n; ++j) b[i][j] = row[j]; }
    string word; cin >> word;
    cout << (sol.exist(b, word) ? 1 : 0) << "\n";
    return 0;
}

