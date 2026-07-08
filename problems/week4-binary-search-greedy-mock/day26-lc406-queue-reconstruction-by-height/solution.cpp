// ============================================================================
// LC-406: Queue Reconstruction by Height
// Difficulty: Medium
// Priority: P1
// Week 4 / Day 26
// ----------------------------------------------------------------------------
// Given people[i] = [h, k] meaning the person has height h and exactly k people
// in front who are >= height h. Reconstruct and return the queue.
//
// Constraints:
//   - 1 <= people.length <= 2000
//   - 0 <= h <= 1e6
//   - 0 <= k < people.length
//
// Goal: O(n^2) or O(n log n).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Next n lines: h k
//   Print the reconstructed queue, one "h k" per line.
// Expected output for test.in: 5 0 | 7 0 | 5 2 | 6 1 | 4 4 | 7 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
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
    vector<vector<int>> p(n, vector<int>(2));
    for (int i = 0; i < n; ++i) cin >> p[i][0] >> p[i][1];
    auto res = sol.reconstructQueue(p);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\n";
    return 0;
}

