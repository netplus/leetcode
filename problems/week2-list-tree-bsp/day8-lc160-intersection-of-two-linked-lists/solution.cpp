// ============================================================================
// LC-160: Intersection of Two Linked Lists
// Difficulty: Easy
// Priority: P1
// Week 2 / Day 8
// ----------------------------------------------------------------------------
// Given the heads of two singly linked-lists, return the node at which the two
// lists intersect. If no intersection, return null.
//
// Constraints:
//   - number of nodes in listA in [0, 3e4]
//   - number of nodes in listB in [0, 3e4]
//   - 1 <= Node.val <= 1e5
//   - intersectVal is 0 or the value of the intersected node
//
// Goal: O(n+m) time, O(1) space.
//
// Local I/O format (for test.in):
//   Line 1: na (len listA) nb (len listB) (skipA skipB ignored here)
//   Line 2: na values of listA (with shared suffix appended)
//   Line 3: nb values of listB (same shared suffix)
//   Line 4: interLen (number of shared nodes)
//   Build: last interLen nodes are shared. Print the intersected node value, or -1.
// Expected output for test.in: 8
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
    ListNode *getIntersectionNode(ListNode *headA, ListNode *headB) {
        // Your implementation here.
        return nullptr;
    }

};

// ---------- Local test harness ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int na, nb; if (!(cin >> na >> nb)) return 0;
    vector<int> a(na), b(nb);
    for (int i = 0; i < na; ++i) cin >> a[i];
    for (int i = 0; i < nb; ++i) cin >> b[i];
    int interLen; cin >> interLen;
    // Build lists; last interLen nodes are shared.
    ListNode* headA = buildList(a);
    ListNode* headB = buildList(b);
    if (interLen > 0) {
        // walk A to the start of the shared suffix and attach a shared tail from B
        int skipA = na - interLen, skipB = nb - interLen;
        // rebuild with shared tail
        vector<int> shared(a.begin() + skipA, a.end());
        ListNode dA, *tA = &dA;
        for (int i = 0; i < skipA; ++i) { tA->next = new ListNode(a[i]); tA = tA->next; }
        ListNode* sharedHead = buildList(shared);
        tA->next = sharedHead;
        ListNode dB, *tB = &dB;
        for (int i = 0; i < skipB; ++i) { tB->next = new ListNode(b[i]); tB = tB->next; }
        tB->next = sharedHead;
        headA = dA.next; headB = dB.next;
    }
    ListNode* ans = sol.getIntersectionNode(headA, headB);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}

