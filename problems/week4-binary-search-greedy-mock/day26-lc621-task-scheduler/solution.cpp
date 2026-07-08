// ============================================================================
// LC-621: Task Scheduler
// Difficulty: Medium
// Priority: P1
// Week 4 / Day 26
// ----------------------------------------------------------------------------
// Given a tasks array of characters and an integer n (cooldown), the CPU needs n
// units of idle time between two same tasks. Return the minimum units of time to
// finish all tasks.
//
// Constraints:
//   - 1 <= tasks.length <= 1e4
//   - tasks[i] is an uppercase English letter
//   - 0 <= n <= 100
//
// Goal: O(n) time (greedy by count).
//
// Local I/O format (for test.in):
//   Line 1: n (number of tasks) cooldown
//   Line 2: a string of n characters (the tasks)
//   Print the minimum units of time.
// Expected output for test.in: 8
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    int leastInterval(vector<char>& tasks, int n) {
        // Your implementation here.
        return 0;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nt, cooldown; if (!(cin >> nt >> cooldown)) return 0;
    string s; cin >> s;
    vector<char> tasks(s.begin(), s.end());
    cout << sol.leastInterval(tasks, cooldown) << "\n";
    return 0;
}

