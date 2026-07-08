// ============================================================================
// LC-142: Linked List Cycle II
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 9
// ----------------------------------------------------------------------------
// Given head, return the node where the cycle begins. If no cycle, return null.
//
// Constraints:
//   - number of nodes in [0, 1e4]
//   - -1e5 <= Node.val <= 1e5
//   - pos is -1 or a valid index
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n pos
//   Line 2: n values
//   Print the value of the cycle-entry node, or -1.
// Expected output for test.in: 2
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
    ListNode *detectCycle(ListNode *head) {
        // Your implementation here.
        return nullptr;
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
    ListNode* ans = sol.detectCycle(head);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}

