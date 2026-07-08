// ============================================================================
// LC-143: Reorder List
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 14
// ----------------------------------------------------------------------------
// Reorder the list so that it becomes L0 -> Ln -> L1 -> Ln-1 -> L2 -> Ln-2 -> ...
// You may not modify the values in the list's nodes; only nodes themselves may be
// changed.
//
// Constraints:
//   - number of nodes in [1, 5e4]
//   - 1 <= Node.val <= 1000
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n values
//   Print the reordered list values space-separated.
// Expected output for test.in: 1 4 2 3
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
    void reorderList(ListNode* head) {
        // Your implementation here.
        return ;
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
    ListNode* head = buildList(a);
    sol.reorderList(head);
    printList(head);
    return 0;
}

