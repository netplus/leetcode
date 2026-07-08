// ============================================================================
// LC-134: Gas Station
// Difficulty: Medium
// Priority: P1
// Week 4 / Day 24
// ----------------------------------------------------------------------------
// There are n gas stations along a circular route. gas[i] is gas at station i;
// cost[i] is gas to travel from i to i+1. Return the starting gas station index if
// you can travel around once, or -1. If a solution exists, it is unique.
//
// Constraints:
//   - n == gas.length == cost.length
//   - 1 <= n <= 1e5
//   - 0 <= gas[i], cost[i] <= 1e4
//
// Goal: O(n) time, O(1) space (greedy).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n gas values
//   Line 3: n cost values
//   Print the starting index, or -1.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        // Your implementation here.
        return -1;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> gas(n), cost(n);
    for (int i = 0; i < n; ++i) cin >> gas[i];
    for (int i = 0; i < n; ++i) cin >> cost[i];
    cout << sol.canCompleteCircuit(gas, cost) << "\n";
    return 0;
}

