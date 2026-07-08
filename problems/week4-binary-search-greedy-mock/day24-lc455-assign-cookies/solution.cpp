// ============================================================================
// LC-455: Assign Cookies
// Difficulty: Easy
// Priority: P0
// Week 4 / Day 24
// ----------------------------------------------------------------------------
// Assume you are an awesome parent giving cookies to children. Each child i has a
// greed factor g[i]; each cookie j has size s[j]. A child is content if s[j] >= g[i].
// Maximize the number of content children (one cookie per child).
//
// Constraints:
//   - 1 <= g.length <= 3e4
//   - 0 <= s.length <= 3e4
//   - 1 <= g[i], s[j] <= 2^31 - 1
//
// Goal: O(g log g + s log s) time.
//
// Local I/O format (for test.in):
//   Line 1: ng ns
//   Line 2: ng space-separated integers (g)
//   Line 3: ns space-separated integers (s)
//   Print the number of content children.
// Expected output for test.in: 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int findContentChildren(vector<int>& g, vector<int>& s) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int ng, ns; if (!(cin >> ng >> ns)) return 0;
    vector<int> g(ng), s(ns);
    for (int i = 0; i < ng; ++i) cin >> g[i];
    for (int i = 0; i < ns; ++i) cin >> s[i];
    cout << sol.findContentChildren(g, s) << "\n";
    return 0;
}

