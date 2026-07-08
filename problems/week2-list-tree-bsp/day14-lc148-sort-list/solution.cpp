// ============================================================================
// LC-148: Sort List
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 14
// ----------------------------------------------------------------------------
// Given the head of a linked list, sort it in ascending order using O(n log n)
// time and O(1) extra space (follow-up).
//
// Constraints:
//   - number of nodes in [0, 5e4]
//   - -1e5 <= Node.val <= 1e5
//
// Goal: O(n log n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n values
//   Print the sorted list values space-separated.
// Expected output for test.in: 1 2 3 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

struct ListNode {
    int val;
    ListNode *next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode *next) : val(x), next(next) {}
};

static ListNode* buildList(const vector<int>& a) {
    ListNode dummy; ListNode* tail = &dummy;
    for (int v : a) { tail->next = new ListNode(v); tail = tail->next; }
    return dummy.next;
}
static void printList(ListNode* h) {
    bool first = true;
    for (ListNode* c = h; c; c = c->next) { if (!first) cout << " "; cout << c->val; first = false; }
    cout << "\n";
}

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    ListNode* sortList(ListNode* head) {
        // Your implementation here.
        return nullptr;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printList(sol.sortList(buildList(a)));
    return 0;
}

