// ============================================================================
// LC-92: Reverse Linked List II
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 8
// ----------------------------------------------------------------------------
// Given the head of a singly linked list and two integers left and right
// (1-indexed), reverse the nodes from position left to position right, and
// return the reversed list.
//
// Constraints:
//   - number of nodes == n
//   - 1 <= n <= 500
//   - 1 <= left <= right <= n
//   - -500 <= Node.val <= 500
//
// Goal: O(n) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n left right
//   Line 2: n space-separated node values
//   Print the resulting list values space-separated.
// Expected output for test.in: 1 4 3 2 5
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
    ListNode* reverseBetween(ListNode* head, int left, int right) {
        // Your implementation here.
        return nullptr;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, left, right; if (!(cin >> n >> left >> right)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    ListNode* head = buildList(a);
    printList(sol.reverseBetween(head, left, right));
    return 0;
}

