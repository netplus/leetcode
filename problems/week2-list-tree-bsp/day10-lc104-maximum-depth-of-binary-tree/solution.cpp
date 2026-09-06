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
// 题目示例：
// 示例 1：
//   Input: root = [3,9,20,null,null,15,7]
//   Output: 3
//
// 示例 2：
//   Input: root = [1,null,2]
//   Output: 2
//
// 约束与要求：
//   - 树中节点的数量在 [0, 10^4] 区间内。
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜从题目直觉到可复用算法
//
// 1. 题目直觉 / 图形模型
//   最大深度可以从根向下理解，也可以从叶子向上汇总：
//
//           root
//          /       left    right
//
//   root 的答案只取决于两个孩子各自能向下走多深：较深的一边再加上 root 自己这一层。
//
// 2. 最自然的一般性解法
//   定义 D(node)=以 node 为根的子树最大深度。空节点深度为 0；非空节点先分别得到左右子树深度，再返回 1+max(left,right)。每个节点只需计算一次。
//
// 3. 一般解法的问题与限制
//   这个递归关系已经是 O(n) 的自然最优解，没有需要额外消除的重复子问题。真正需要先固定的是‘深度按节点数还是边数计’以及空树基例，否则公式里的 +1 很容易产生口径错误。
//
// 4. 从具体问题抽象规律
//   树题经常可以把整棵树的问题压成‘每棵子树向父节点返回一个摘要’。先定义返回值的物理意义，再由孩子摘要组合当前答案。
//
// 5. 必要背景知识
//   “深度/高度”的计数约定在不同资料中可能按节点数，也可能按边数，所以必须先固定本题口径。
//
//   LeetCode LC-104 把最大深度定义为：从根到最远叶子这条路径上的节点数。
//   空树深度 = 0；只有根节点的树深度 = 1。
//
//   因此当前递归状态 D(node) 也按节点数计：`D(node)=1+max(D(left),D(right))`。
//
// 6. 专项算法｜树形递归：先定义子树返回值，再由左右答案合成当前节点
//   前面的推理已经得到可复用机制；现在才给它一个检索名称：后序式树形汇总。通用机制见 docs/algorithms/tree-traversal.md。回到本题时，只需要把其中的状态、边界和更新事件映射到当前题意，不要反过来用模板猜题。
//
// 7. 核心算法
//   定义 maxDepth(node) 为“以 node 为根的子树深度”；父节点只需取左右子树较大值再加自己这一层。
//
// 8. 公式 / 不变量
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
// 9. 执行步骤
//   1. 遇到空节点返回 0，这是最小子问题，也是递归终点
//   2. 递归取得左子树最大深度和右子树最大深度
//   3. 当前节点必须占一层，因此返回 1 + max(leftDepth, rightDepth)
//   4. 根节点拿到的返回值就是整棵树的最大深度
//   记忆：孩子报深度，父亲选更深的一边再 +1。
//
// 10. 为什么不会漏 / 不会重
//   任意从 node 到叶子的路径第一步只能进入左子树或右子树，因此最长路径必等于两边最长路径中较大的那条再加当前节点。空树深度为 0，叶节点因此得到 1。这个递推枚举了路径唯一可能的两个方向，没有其它路径可漏，也不会把左右两条路径同时相加造成重复。
//
// 11. 边界与易错点
//   本题深度按“节点数”计，所以空树 0、叶子 1。不要误写成 left+right+1，那计算的是经过当前节点拼接两边的路径规模，会导向 Day 11 的直径问题。极端链状树递归深度可达 n；若环境栈限制严格可以改用 LC-102 的 BFS 层数。
//
// 12. 举一反三
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
        // 函数契约：maxDepth(node) 返回“以 node 为根的子树最大深度”，按路径上的节点数计。
        // 空子树不贡献任何节点，因此深度为 0；这个边界也让叶子节点自然得到 1。
        if (!root) return 0;

        // 从当前节点到最远叶子的路径只能选择左、右子树中的一条继续向下；
        // 两个递归调用分别完整计算左右子树深度，父节点取更深的一边，再把自己这一层 +1。
        // 这里不能写成 left+right+1：左右相加是在拼一条经过当前节点的双臂路径，那是 LC-543 直径的模型。
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
