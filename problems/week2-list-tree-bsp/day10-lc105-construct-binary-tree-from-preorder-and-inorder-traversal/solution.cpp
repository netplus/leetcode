// ============================================================================
// LC-105: Construct Binary Tree from Preorder and Inorder Traversal
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 10
// ----------------------------------------------------------------------------
// Given two integer arrays preorder and inorder where preorder is the
// preorder traversal of a binary tree and inorder is the inorder traversal of
// the same tree, construct and return the binary tree.
//
// Constraints:
//   - 1 <= preorder.length <= 3000
//   - inorder.length == preorder.length
//   - all values unique
//   - -3000 <= preorder[i], inorder[i] <= 3000
//
// Goal: O(n) time (with hash map).
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n preorder values
//   Line 3: n inorder values
//   Print the constructed tree's level order (-1 = null).
// Expected output for test.in: 3 9 20 -1 -1 15 7
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

// ---------- Solution (implement this) ----------
class Solution {
public:
    // TODO: implement
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
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
    vector<int> pre(n), in(n);
    for (int i = 0; i < n; ++i) cin >> pre[i];
    for (int i = 0; i < n; ++i) cin >> in[i];
    TreeNode* root = sol.buildTree(pre, in);
    printTree(root);
    return 0;
}

