// ============================================================================
// LC-234: Palindrome Linked List
// Difficulty: Easy
// Priority: P1
// Week 2 / Day 9
// ----------------------------------------------------------------------------
// Given the head of a singly linked list, return true if it is a palindrome.
//
// Constraints:
//   - number of nodes in [1, 1e5]
//   - 0 <= Node.val <= 9
//
// Goal: O(n) time, O(1) space (follow-up).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n values
//   Print 1 if palindrome, else 0.
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
    bool isPalindrome(ListNode* head) {
        // Your implementation here.
        return false;
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
    cout << (sol.isPalindrome(buildList(a)) ? 1 : 0) << "\n";
    return 0;
}

