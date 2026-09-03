// ============================================================================
// LC-543：二叉树的直径
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 11 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一棵二叉树的根节点，返回该树的直径。
// 二叉树的直径是指树中任意两个节点之间最长路径的长度。
// 这条路径可能经过也可能不经过根节点 root。
// 两节点之间路径的长度由它们之间边数表示。
//
// 约束与要求：
//   - 树中节点数目在范围 [1, 10^4] 内
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜后序树形 DP：高度与直径分工
// - 核心要点：
//   1. 思路起点：递归函数向父节点返回单边最大高度；当前节点可作为路径最高点，用 leftHeight+rightHeight 更新全局直径。
//   2. 执行逻辑：1. 空节点高度返回 0；2. 后序取得左右高度并更新直径；3. 向上返回 1+较大子树高度。
//   3. 为什么这样做：任意简单路径都有唯一最高节点；当递归到它时，左右最大向下路径之和覆盖该路径的最优选择，因此全局最大不会遗漏。
// - 边界与易错点：直径按边数计，所以节点高度相加无需再加 1；返回给父节点的路径不能同时走左右两边。
// - 举一反三：树上任意到任意路径常用“当前可闭合双臂、向父只能上报单臂”的二元角色设计。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个层序节点值（-1 表示空节点）。
//   输出：二叉树直径（边数）。
// test.in 的预期输出：3
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
    int diameter = 0;

    int height(TreeNode* node) {
        if (!node) return 0;
        const int leftHeight = height(node->left);
        const int rightHeight = height(node->right);
        diameter = max(diameter, leftHeight + rightHeight);
        return 1 + max(leftHeight, rightHeight);
    }

public:
    int diameterOfBinaryTree(TreeNode* root) {
        diameter = 0;
        height(root);
        return diameter;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<long long> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.diameterOfBinaryTree(buildTreeLO(a)) << "\n";
    return 0;
}

