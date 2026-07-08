// ============================================================================
// LC-28: Find the Index of the First Occurrence in a String
// Difficulty: Easy
// Priority: P1
// Week 1 / Day 5
// ----------------------------------------------------------------------------
// Given two strings haystack and needle, return the index of the first
// occurrence of needle in haystack, or -1 if needle is not part of haystack.
//
// Constraints:
//   - 1 <= haystack.length, needle.length <= 1e4
//   - haystack and needle consist of lowercase English letters.
//
// Goal: O(n+m) (KMP) or O(n*m) naive.
//
// Local I/O format (for test.in):
//   Line 1: haystack
//   Line 2: needle
//   Print the index (0-based) or -1.
// Expected output for test.in: 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int strStr(string haystack, string needle) {
        // Your implementation here.
        return -1;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    string h, n; if (!(cin >> h >> n)) return 0;
    cout << sol.strStr(h, n) << "\n";
    return 0;
}

