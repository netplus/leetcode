// ============================================================================
// LC-98: Validate Binary Search Tree
// Difficulty: Medium
// Priority: P1
// Week 2 / Day 11
// ----------------------------------------------------------------------------
// Given the root of a binary tree, determine if it is a valid binary search
// tree (BST).
//
// Constraints:
//   - number of nodes in [1, 1e4]
//   - -2^31 <= Node.val <= 2^31 - 1
//
// Goal: O(n) time.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n level-order values (-1 = null)
//   Print 1 if valid BST, else 0.
// Expected output for test.in: 0
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
    bool isValidBST(TreeNode* root) {
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
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.isValidBST(buildTreeLO(a)) ? 1 : 0) << "\n";
    return 0;
}

