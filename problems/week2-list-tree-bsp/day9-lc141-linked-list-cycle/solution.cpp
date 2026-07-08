// ============================================================================
// LC-141: Linked List Cycle
// Difficulty: Easy
// Priority: P0
// Week 2 / Day 9
// ----------------------------------------------------------------------------
// Given head, determine if the linked list has a cycle in it. Return true if
// there is a cycle, else false.
//
// Constraints:
//   - number of nodes in [0, 1e4]
//   - -1e5 <= Node.val <= 1e5
//   - pos is -1 or a valid index (cycle entry)
//
// Goal: O(n) time, O(1) space (fast/slow).
//
// Local I/O format (for test.in):
//   Line 1: n pos
//   Line 2: n values
//   pos = index where tail connects (-1 = no cycle)
//   Print 1 if cycle, else 0.
// Expected output for test.in: 1
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
    bool hasCycle(ListNode *head) {
        // Your implementation here.
        return false;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, pos; if (!(cin >> n >> pos)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    if (pos >= 0) {
        ListNode* tail = head; while (tail->next) tail = tail->next;
        ListNode* p = head; for (int i = 0; i < pos; ++i) p = p->next;
        tail->next = p;
    }
    cout << (sol.hasCycle(head) ? 1 : 0) << "\n";
    return 0;
}

