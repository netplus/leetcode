// ============================================================================
// LC-155: Min Stack
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 12
// ----------------------------------------------------------------------------
// Design a stack that supports push, pop, top, and retrieving the minimum
// element in O(1) time. (LC class MinStack; here modeled as Solution.)
//
// Constraints:
//   - -2^31 <= val <= 2^31 - 1
//   - methods pop/top/getMin called on non-empty stack
//   - at most 3e4 calls
//
// Goal: O(1) per operation.
//
// Local I/O format (for test.in):
//   Line 1: q (number of operations)
//   Next q lines: op [arg]
//     op = 1 x  -> push(x)
//     op = 2    -> pop()
//     op = 3    -> top()  -> print x
//     op = 4    -> getMin() -> print min
// Expected output for test.in: -2 0 -3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

#include <climits>

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    void push(int val) {
        // Your implementation here.
        return ;
    }
    // Implement these as the MinStack operations:
    void pop() { /* TODO */ }
    int top() { /* TODO */ return 0; }
    int getMin() { /* TODO */ return 0; }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int q; if (!(cin >> q)) return 0;
    while (q--) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; sol.push(x); }
        else if (op == 2) { sol.pop(); }
        else if (op == 3) { cout << sol.top() << "\n"; }
        else { cout << sol.getMin() << "\n"; }
    }
    return 0;
}

