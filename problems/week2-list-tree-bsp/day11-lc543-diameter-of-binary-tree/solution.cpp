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
// 解法精讲｜树形 DP 双角色：向父只能交一条臂，当前节点却可以把左右两臂拼成完整路径
//
// 0. 优化是怎么来的
//   一个直接思路是枚举每个节点作为路径最高点，再分别计算它左、右子树的高度，用 leftHeight+rightHeight 更新直径。逻辑正确，但若每个节点都重新递归求高度，同一棵子树会被多个祖先重复计算，最坏 O(n^2)。
//
//   后序遍历本来就会先完成孩子再回到父亲，所以可以让每棵子树只计算一次高度，并在返回父节点前顺手上报这个高度；当前节点拿到左右高度时同时完成一次直径候选更新。
//
//   于是“求高度”和“枚举最高点”被融合到同一趟 DFS。
//
//   优化类型：重复子问题 -> 后序返回可复用状态。每个节点只处理一次，O(n)。
//
// 1. 图像直觉
//   直接复用 Day 10 / LC-104 的“子树向父返回高度”，但这次多问一个问题：
//   当前节点能不能把左右两条向下路径接起来？
//
//           1
//          / \
//         2   3
//        / \
//       4   5
//
//   站在节点 2：
//   左臂：2 -> 4，下面贡献 leftHeight=1
//   右臂：2 -> 5，下面贡献 rightHeight=1
//
//   经过 2 的完整路径可以是：
//   4 -> 2 -> 5
//   长度（边数）= 1 + 1 = 2
//
//   但当节点 2 把信息交给父节点 1 时，不能同时把 4 和 5 两条路都带上去；
//   否则路径会在 2 处分叉，不再是一条简单路径。
//   所以向父只能返回更长的一条：
//   height(2) = 1 + max(1,1) = 2。
//
// 2. 一句话核心
//   当前节点可以用 leftHeight+rightHeight 闭合一条完整直径候选；但向父节点只能返回一条最长单臂高度。
//
// 3. 公式 / 不变量
//   令 H(node) = 从 node 向下走到最深节点的最大“节点高度”，空节点 H=0。
//
//   left  = H(node->left)
//   right = H(node->right)
//
//   经过当前 node 的直径候选（边数）：
//   candidate = left + right
//   answer = max(answer, candidate)
//
//   向父返回：
//   H(node) = 1 + max(left, right)
//
//   两个角色必须分开：
//   全局候选可以左右都要；
//   返回父节点只能选择一边。
//
// 4. 执行步骤
//   1. 空节点高度为 0，作为递归基线
//   2. 后序计算 leftHeight 和 rightHeight，确保两棵子树的信息已经可用
//   3. 用 leftHeight + rightHeight 更新全局 diameter，表示以当前节点为最高点的最佳路径
//   4. 向父节点返回 1 + max(leftHeight, rightHeight)，因为父路径经过当前节点后只能继续进入一侧
//   记忆：当前节点能“两臂抱满”，向父汇报时却只能“举一只手”。
//
// 5. 为什么不会漏 / 不会重
//   任意两节点之间的简单路径都有唯一的最高汇合节点。设这条最优路径的最高点为 x，那么它在 x 的左、右方向（某一侧也可能为空）所能取得的最长下行路径，正是在计算 x 时得到的 leftHeight/rightHeight，因此 candidate 会覆盖这条最优路径。遍历所有节点取最大值不会漏。每个节点高度只计算一次，所以不会重复。
//
// 6. 边界与易错点
//   直径按边数计，而这里空节点高度定义为 0，所以 leftHeight+rightHeight 已经正好是边数，不要再 +1。向父返回值绝不能是 left+right+1，那会允许一条路径在子节点处分叉。
//
// 7. 举一反三
//   这题是 Day 10 LC-104 的直接升级：LC-104 只关心“向上还能延伸多长”，LC-543 增加“在当前节点能闭合多长”。Day 14 LC-124 最大路径和会几乎原样复用这个双角色框架，只把高度换成路径和，并处理负贡献。
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
