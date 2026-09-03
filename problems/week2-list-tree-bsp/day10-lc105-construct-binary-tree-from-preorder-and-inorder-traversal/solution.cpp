// ============================================================================
// LC-105：从前序与中序遍历序列构造二叉树
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定两个整数数组 preorder 和 inorder，其中 preorder 是二叉树的先序遍历，inorder 是同一棵树的中序遍历，请构造二叉树并返回其根节点。
//
// 约束与要求：
//   - 1 <= preorder.length <= 3000
//   - inorder.length == preorder.length
//   - -3000 <= preorder[i], inorder[i] <= 3000
//   - preorder 和 inorder 均无重复元素
//   - inorder 均出现在 preorder
//   - preorder 保证为二叉树的前序遍历序列
//   - inorder 保证为二叉树的中序遍历序列
//
// 复杂度目标：O(n) 时间（使用哈希表）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜前序定根 + 中序切分
// - 核心要点：
//   1. 思路起点：前序首元素确定当前根；根在中序中的位置把节点集合唯一切成左、右子树，子树规模又确定前序片段。
//   2. 执行逻辑：1. 预建中序值到下标的哈希表；2. 按前序顺序取根并在中序区间切分；3. 递归构建左区间和右区间。
//   3. 为什么这样做：节点值唯一时，中序切点唯一；前序在根后先列完整左子树再列右子树，递归按中序区间大小消费前序元素，故结构唯一还原。
// - 边界与易错点：必须先构建左子树再右子树以匹配前序游标；空区间返回 nullptr；成员游标每次调用前要重置。
// - 举一反三：任意两种包含中序的遍历通常可重建树；后序则从末尾取根并优先构建右子树。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个前序遍历值。
//   第 3 行：n 个中序遍历值。
//   输出：构造后二叉树的层序序列（-1 表示空节点）。
// test.in 的预期输出：3 9 20 -1 -1 15 7
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

[[maybe_unused]] static TreeNode* buildTreeLO(const vector<long long>& a) {
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
[[maybe_unused]] static void printTree(TreeNode* root) {
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

// ---------- 题解实现 ----------
class Solution {
    unordered_map<int, int> inorderIndex;
    int preorderIndex = 0;

    TreeNode* build(const vector<int>& preorder, int left, int right) {
        if (left > right) return nullptr;
        const int rootValue = preorder[preorderIndex++];
        TreeNode* root = new TreeNode(rootValue);
        const int middle = inorderIndex[rootValue];
        root->left = build(preorder, left, middle - 1);
        root->right = build(preorder, middle + 1, right);
        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inorderIndex.clear();
        preorderIndex = 0;
        for (int i = 0; i < static_cast<int>(inorder.size()); ++i) {
            inorderIndex[inorder[i]] = i;
        }
        return build(preorder, 0, static_cast<int>(inorder.size()) - 1);
    }
};

// ---------- 本地测试适配器 ----------
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

