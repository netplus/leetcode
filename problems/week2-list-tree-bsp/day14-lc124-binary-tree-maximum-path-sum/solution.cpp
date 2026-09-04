// ============================================================================
// LC-124：二叉树中的最大路径和
// 难度：困难
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 14 天
// ----------------------------------------------------------------------------
// 题目描述：
// 二叉树中的路径被定义为一条节点序列，序列中每对相邻节点之间都存在一条边。
// 同一个节点在一条路径序列中至多出现一次。
// 该路径至少包含一个节点，且不一定经过根节点。
// 路径和是路径中各节点值的总和。
// 给你一个二叉树的根节点 root，返回其最大路径和。
//
// 约束与要求：
//   - 树中节点数目范围是 [1, 3 * 10^4]
//   - -1000 <= Node.val <= 1000
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜树形 DP：当前节点闭合双臂，向父只返回单臂最大贡献
//
// 0. 优化是怎么来的
//   一个直接但代价较高的思路，是把每个节点都当成路径最高点，再分别重新计算它左、右子树能提供的最大向下路径和。这样当然能覆盖答案，但祖先和子节点会反复计算同一棵子树的最佳向下贡献，最坏可到 O(n^2)。
//
//   Day 11 的 LC-543 已经告诉我们怎样消掉这类重复子问题：后序遍历时让每棵子树只计算一次“向父还能延伸多少”，并把结果直接上报。当前节点收到左右两个上报值后，顺手计算一次“经过我闭合的完整候选”。
//
//   本题比 LC-543 多出的关键只有负数：如果某侧向下贡献 < 0，把它接进路径只会让总和更差，因此对当前节点而言，这一侧等价于贡献 0，可以永久丢弃。
//
//   优化类型：重复子树计算 -> 后序状态复用；负贡献被支配 -> 截断为 0。最后才把这套机制称为树形 DP。
//
// 1. 图像直觉
//   看经典局部结构：
//
//             20
//            /  \
//          15    7
//
//   左右子树向 20 汇报：
//   leftGain  = 15
//   rightGain = 7
//
//   站在 20 自己这里，可以把两边都接起来：
//   15 -> 20 -> 7
//   candidate = 15 + 20 + 7 = 42
//
//   但 20 若继续向父节点汇报，就不能把 15 和 7 两边同时带上去；否则路径会在 20 分叉。
//   所以只能返回：
//   20 + max(15, 7) = 35
//
//   再看负贡献：
//
//             5
//            /
//          -8
//
//   把 -8 接进路径只会让 5 变差，所以对 5 来说：
//   leftGain = max(0, -8) = 0
//
//   这不是“忽略一个节点”，而是说：当前完整路径可以选择不进入那条负收益分支。
//
// 2. 一句话核心
//   每棵子树只向父节点汇报一条最大单臂贡献；当前节点把左右正贡献都接上更新全局答案，再只选较好的一侧继续向上。
//
// 3. 公式 / 不变量
//   令 gain(node) 表示：
//   必须从 node 出发，并且向下最多选择一侧时，能提供给父节点的最大路径和。
//
//   left  = max(0, gain(node->left))
//   right = max(0, gain(node->right))
//
//   以当前节点为最高点的完整路径候选：
//   candidate = node->val + left + right
//   best = max(best, candidate)
//
//   向父节点返回：
//   gain(node) = node->val + max(left, right)
//
//   两个角色必须分开：
//   - 更新全局答案时，左右两臂可以同时使用；
//   - 向父返回时，只能选一臂，否则路径会分叉。
//
//   best 必须初始化为 -∞（实现用 INT_MIN），不能初始化为 0，因为整棵树可能全是负数。
//
// 4. 执行步骤
//   1. 后序进入 gain(node)：空节点返回 0，让父节点可以自然把不存在的分支视为无贡献
//   2. 递归得到左右子树的单臂贡献，并分别与 0 取最大值；负贡献不值得接入当前路径
//   3. 用 node->val + left + right 更新全局 best，表示把当前节点作为最高汇合点时的最佳完整路径
//   4. 向父节点返回 node->val + max(left,right)，只允许一侧继续延伸，保持结果仍是一条简单路径
//   5. 根节点递归结束后返回全局 best；它可能来自任意子树，并不要求经过 root
//   记忆：当前节点可以双臂闭合；向父只能单臂上报；负贡献不接。
//
// 5. 为什么不会漏 / 不会重
//   任意一条非空简单路径都有唯一的最高汇合节点 x。若路径经过 x 的左右两侧，那么在 x 看来，它的两部分分别不会优于 left/right 所代表的最佳向下贡献；若某侧贡献为负，舍弃它只会更优。因此处理 x 时的 candidate 一定覆盖以 x 为最高点的最佳路径。遍历所有节点，就覆盖了全局最优路径。向父只返回一侧，是因为父节点若继续接这条路径，经过当前节点后只能进入一个孩子，否则会形成分叉而不再是简单路径。
//
// 6. 边界与易错点
//   最常见错误是把 best 初始化为 0，这会让单节点负数树错误返回 0；题目要求路径至少包含一个节点，所以必须让真实负节点有机会成为答案。另一个错误是向父返回 node+left+right——那是当前节点的完整候选，不是可继续延伸的单臂。`max(0, gain(child))` 的含义是“可以不走这条负分支”，不是说负节点不存在。
//
// 7. 举一反三
//   这是 LC-543 的几乎一比一升级：LC-543 的 child state 是高度，LC-124 的 child state 是最大向下路径和；LC-543 不需要处理负高度，LC-124 需要把负贡献截断为 0。以后遇到树上最长/最大路径问题，优先问两个问题：当前节点能不能把左右两侧闭合？向父又允许带几条分支？
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个层序节点值（-1 表示空节点）。
//   输出：最大路径和。
// test.in 的预期输出：6
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
    int best = INT_MIN;

    int gain(TreeNode* node) {
        if (!node) return 0;
        const int left = max(0, gain(node->left));
        const int right = max(0, gain(node->right));
        best = max(best, node->val + left + right);
        return node->val + max(left, right);
    }

public:
    int maxPathSum(TreeNode* root) {
        best = INT_MIN;
        gain(root);
        return best;
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
    cout << sol.maxPathSum(buildTreeLO(a)) << "\n";
    return 0;
}
