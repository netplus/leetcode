// ============================================================================
// LC-23: Merge k Sorted Lists
// Difficulty: Hard
// Priority: P1
// Week 4 / Day 25
// ----------------------------------------------------------------------------
// Given an array of k linked lists, each sorted in ascending order, merge them
// into one sorted linked list and return its head.
//
// Constraints:
//   - k == lists.length
//   - 0 <= k <= 1e4
//   - 0 <= lists[i].length <= 500
//   - -1e4 <= Node.val <= 1e4
//
// Goal: O(N log k) time (min-heap).
//
// Local I/O format (for test.in):
//   Line 1: k
//   Then k blocks: each block is a line with li (length) then a line with li values
//   Print the merged list values space-separated.
// Expected output for test.in: 1 1 2 3 4 4 5 6
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
    ListNode* mergeKLists(vector<ListNode*>& lists) {
        // Your implementation here.
        return nullptr;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int k; if (!(cin >> k)) return 0;
    vector<ListNode*> lists;
    for (int i = 0; i < k; ++i) {
        int li; cin >> li;
        vector<int> v(li);
        for (int j = 0; j < li; ++j) cin >> v[j];
        lists.push_back(buildList(v));
    }
    printList(sol.mergeKLists(lists));
    return 0;
}

