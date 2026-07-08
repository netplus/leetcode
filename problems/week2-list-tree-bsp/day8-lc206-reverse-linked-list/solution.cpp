// ============================================================================
// LC-206: Reverse Linked List
// Difficulty: Easy
// Priority: P0
// Week 2 / Day 8
// ----------------------------------------------------------------------------
// Given the head of a singly linked list, reverse the list, and return the
// reversed list.
//
// Constraints:
//   - number of nodes in [0, 5000]
//   - -5000 <= Node.val <= 5000
//
// Goal: O(n) time, O(1) space (iterative).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated node values
//   Print the reversed list values space-separated.
// Expected output for test.in: 5 4 3 2 1
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
    ListNode* reverseList(ListNode* head) {
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
    ListNode* head = buildList(a);
    printList(sol.reverseList(head));
    return 0;
}

