// ============================================================================
// LC-22: Generate Parentheses
// Difficulty: Medium
// Priority: P1
// Week 2 / Day 13
// ----------------------------------------------------------------------------
// Given n pairs of parentheses, write a function to generate all combinations
// of well-formed parentheses.
//
// Constraints:
//   - 1 <= n <= 8
//
// Goal: O(4^n / sqrt(n)).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Print each combination on its own line.
// Expected output for test.in: ((())) | (()()) | (())() |()(()) | ()()()
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<string> generateParenthesis(int n) {
        // Your implementation here.
        return {};
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    auto res = sol.generateParenthesis(n);
    for (auto& s : res) cout << s << "\n";
    return 0;
}

