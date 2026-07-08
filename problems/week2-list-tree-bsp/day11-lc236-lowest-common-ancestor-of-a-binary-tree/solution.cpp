// ============================================================================
// LC-236: Lowest Common Ancestor of a Binary Tree
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 11
// ----------------------------------------------------------------------------
// Given a binary tree, find the lowest common ancestor (LCA) of two given
// nodes p and q. The LCA is the lowest node that has both p and q as descendants.
//
// Constraints:
//   - number of nodes in [2, 1e5]
//   - -1e9 <= Node.val <= 1e9
//   - all Node.val are unique
//   - p != q and both exist in the tree
//
// Goal: O(n) time.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Line 3: p q (values of the two nodes)
//   Print the LCA node value.
// Expected output for test.in: 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode() : val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x) : val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode *left, TreeNode *right) : val(x), left(left), right(right) {}
};

static TreeNode* buildTreeLO(const vector<long long>& a) {
    if (a.empty() || a[0] == -1) return nullptr;
    TreeNode* root = new TreeNode((int)a[0]);
    queue<TreeNode*> q; q.push(root);
    size_t i = 1;
    while (!q.empty() && i < a.size()) {
        TreeNode* cur = q.front(); q.pop();
        if (i < a.size() && a[i] != -1) { cur->left = new TreeNode((int)a[i]); q.push(cur->left); }
        i++;
        if (i < a.size() && a[i] != -1) { cur->right = new TreeNode((int)a[i]); q.push(cur->right); }
        i++;
    }
    return root;
}
static void printTree(TreeNode* root) {
    // Level-order with -1 for null; trailing nulls trimmed.
    if (!root) { cout << "\n"; return; }
    queue<TreeNode*> q; q.push(root);
    vector<long long> out;
    while (!q.empty()) {
        auto* n = q.front(); q.pop();
        if (n) { out.push_back(n->val); q.push(n->left); q.push(n->right); }
        else out.push_back(-1);
    }
    while (!out.empty() && out.back() == -1) out.pop_back();
    for (size_t i = 0; i < out.size(); ++i) cout << out[i] << " \n"[i + 1 == out.size()];
}
static TreeNode* findNode(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (root->val == val) return root;
    TreeNode* l = findNode(root->left, val);
    if (l) return l;
    return findNode(root->right, val);
}

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
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
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    int pv, qv; cin >> pv >> qv;
    TreeNode* root = buildTreeLO(a);
    TreeNode* p = findNode(root, pv);
    TreeNode* q = findNode(root, qv);
    TreeNode* ans = sol.lowestCommonAncestor(root, p, q);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}

