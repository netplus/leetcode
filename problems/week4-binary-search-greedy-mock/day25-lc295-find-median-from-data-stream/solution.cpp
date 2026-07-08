// ============================================================================
// LC-295: Find Median from Data Stream
// Difficulty: Hard
// Priority: P0
// Week 4 / Day 25
// ----------------------------------------------------------------------------
// Implement a class that supports addNum(int num) and findMedian() in O(log n)
// and O(1) respectively. (LC class MedianFinder; here modeled as Solution.)
//
// Constraints:
//   - -1e5 <= num <= 1e5
//   - at most 5e4 calls
//   - findMedian called on non-empty data structure
//
// Goal: O(log n) per addNum (two heaps).
//
// Local I/O format (for test.in):
//   Line 1: q (number of operations)
//   Next q lines: op [arg]
//     op = 1 x  -> addNum(x)
//     op = 2    -> findMedian() -> print median (as float)
// Expected output for test.in: 1.5 2 2.5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

#include <functional>

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    void addNum(int num) {
        // Your implementation here.
        return ;
    }
    // Implement these as the MedianFinder operations:
    double findMedian() { /* TODO */ return 0.0; }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int q; if (!(cin >> q)) return 0;
    while (q--) {
        int op; cin >> op;
        if (op == 1) { int x; cin >> x; sol.addNum(x); }
        else cout << sol.findMedian() << "\n";
    }
    return 0;
}

