// ============================================================================
// LC-21: Merge Two Sorted Lists
// Difficulty: Easy
// Priority: P0
// Week 2 / Day 8
// ----------------------------------------------------------------------------
// Merge two sorted linked lists and return it as a new sorted list.
//
// Constraints:
//   - number of nodes in both lists in [0, 50]
//   - -100 <= Node.val <= 100
//   - both lists sorted in non-decreasing order
//
// Goal: O(n+m) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: n (length of list1)
//   Line 2: n values of list1
//   Line 3: m (length of list2)
//   Line 4: m values of list2
//   Print the merged list values space-separated.
// Expected output for test.in: 1 1 2 3 4 4
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
    ListNode* mergeTwoLists(ListNode* list1, ListNode* list2) {
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
    int m; cin >> m;
    vector<int> b(m);
    for (int i = 0; i < m; ++i) cin >> b[i];
    printList(sol.mergeTwoLists(buildList(a), buildList(b)));
    return 0;
}

