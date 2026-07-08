// ============================================================================
// LC-990: Satisfiability of Equality Equations
// Difficulty: Medium
// Priority: P1
// Week 3 / Day 17
// ----------------------------------------------------------------------------
// Given an array of equations strings of form "a==b" or "a!=b" with single
// lowercase letters, return true if it is possible to assign integers to the
// variables so all equations are satisfied.
//
// Constraints:
//   - 1 <= equations.length <= 500
//   - equations[i].length == 4
//   - equations[i][1] in {'=','!'}; equations[i][0],[3] lowercase letters
//
// Goal: O(n) union-find.
//
// Local I/O format (for test.in):
//   Line 1: n (number of equations)
//   Next n lines: one equation string each
//   Print 1 if satisfiable, else 0.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    bool equationsPossible(vector<string>& equations) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<string> eq(n);
    for (int i = 0; i < n; ++i) cin >> eq[i];
    cout << (sol.equationsPossible(eq) ? 1 : 0) << "\n";
    return 0;
}

