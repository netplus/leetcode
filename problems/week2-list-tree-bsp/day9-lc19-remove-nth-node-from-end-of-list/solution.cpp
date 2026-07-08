// ============================================================================
// LC-19: Remove Nth Node From End of List
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 9
// ----------------------------------------------------------------------------
// Given the head of a linked list, remove the nth node from the end of the
// list and return its head.
//
// Constraints:
//   - number of nodes == sz
//   - 1 <= sz <= 30
//   - 0 <= Node.val <= 100
//   - 1 <= n <= sz
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n (length) k (nth from end to remove)
//   Line 2: n values
//   Print the resulting list values space-separated.
// Expected output for test.in: 1 2 3 5
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
    ListNode* removeNthFromEnd(ListNode* head, int n) {
        // Your implementation here.
        return nullptr;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    printList(sol.removeNthFromEnd(buildList(a), k));
    return 0;
}

