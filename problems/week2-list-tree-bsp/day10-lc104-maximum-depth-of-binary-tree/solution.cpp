// ============================================================================
// LC-104：二叉树的最大深度
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个二叉树 root，返回其最大深度。
// 二叉树的最大深度是指从根节点到最远叶子节点的最长路径上的节点数。
//
// 约束与要求：
//   - 树中节点的数量在 [0, 10^4] 区间内。
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜树形递归：先定义子树返回值，再由左右答案合成当前节点
//
// 前置概念
//   “深度/高度”的计数约定在不同资料中可能按节点数，也可能按边数，所以必须先固定本题口径。
//
//   LeetCode LC-104 把最大深度定义为：从根到最远叶子这条路径上的节点数。
//   空树深度 = 0；只有根节点的树深度 = 1。
//
//   因此当前递归状态 D(node) 也按节点数计：`D(node)=1+max(D(left),D(right))`。
//
// 1. 图像直觉
//   不要先想“怎么遍历整棵树”，先问一个节点向父亲应该汇报什么。
//
//           1
//          / \
//         2   3
//        /
//       4
//
//   站在节点 2：
//   左子树(4) 报告深度 1
//   右空子树报告深度 0
//   所以 2 向上报告 1 + max(1,0) = 2
//
//   站在节点 1：
//   左子树报告 2
//   右子树(3)报告 1
//   所以根报告 1 + max(2,1) = 3
//
//   递归不是“神奇地往下钻”，而是每棵子树完成同一份契约后把结果交给父节点。
//
// 2. 一句话核心
//   定义 maxDepth(node) 为“以 node 为根的子树深度”；父节点只需取左右子树较大值再加自己这一层。
//
// 3. 公式 / 不变量
//   状态定义：
//   D(node) = 以 node 为根，到最远叶子的节点数
//
//   边界：
//   D(nullptr) = 0
//
//   递推：
//   D(node) = 1 + max(D(node->left), D(node->right))
//
//   不变量：每次函数返回时，这棵子树的最大深度已经完整算好，父节点无需知道子树内部细节。
//
// 4. 执行步骤
//   1. 遇到空节点返回 0，这是最小子问题，也是递归终点
//   2. 递归取得左子树最大深度和右子树最大深度
//   3. 当前节点必须占一层，因此返回 1 + max(leftDepth, rightDepth)
//   4. 根节点拿到的返回值就是整棵树的最大深度
//   记忆：孩子报深度，父亲选更深的一边再 +1。
//
// 5. 为什么不会漏 / 不会重
//   任意从 node 到叶子的路径第一步只能进入左子树或右子树，因此最长路径必等于两边最长路径中较大的那条再加当前节点。空树深度为 0，叶节点因此得到 1。这个递推枚举了路径唯一可能的两个方向，没有其它路径可漏，也不会把左右两条路径同时相加造成重复。
//
// 6. 边界与易错点
//   本题深度按“节点数”计，所以空树 0、叶子 1。不要误写成 left+right+1，那计算的是经过当前节点拼接两边的路径规模，会导向 Day 11 的直径问题。极端链状树递归深度可达 n；若环境栈限制严格可以改用 LC-102 的 BFS 层数。
//
// 7. 举一反三
//   这是 Day 11/14 树形 DP 的基础语言：LC-543 直径会复用左右深度，但把 left+right 作为经过当前节点的候选答案；LC-124 最大路径和也会区分“向父节点返回什么”和“当前节点更新什么全局答案”。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的层序节点值（-1 表示空节点）。
//   输出：二叉树的最大深度。
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
public:
    int maxDepth(TreeNode* root) {
        if (!root) return 0;
        return 1 + max(maxDepth(root->left), maxDepth(root->right));
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
    cout << sol.maxDepth(buildTreeLO(a)) << "\n";
    return 0;
}
