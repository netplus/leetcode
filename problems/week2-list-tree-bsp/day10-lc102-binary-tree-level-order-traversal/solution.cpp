// ============================================================================
// LC-102: Binary Tree Level Order Traversal
// Difficulty: Medium
// Priority: P0
// Week 2 / Day 10
// ----------------------------------------------------------------------------
// Given the root of a binary tree, return the level order traversal of its
// nodes' values (i.e., from left to right, level by level).
//
// Constraints:
//   - number of nodes in [0, 2000]
//   - -1000 <= Node.val <= 1000
//
// Goal: O(n) time.
//
// Local I/O format (for test.in):
//   Line 1: n
//   Line 2: n space-separated integers (level order, -1 = null)
//   Print one level per line, space-separated.
// Expected output for test.in: 3 | 9 20 | 15 7
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        // Your implementation here.
        return {};
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
    auto res = sol.levelOrder(buildTreeLO(a));
    for (auto& lvl : res) {
        for (size_t i = 0; i < lvl.size(); ++i) cout << lvl[i] << " \n"[i + 1 == lvl.size()];
    }
    return 0;
}

