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
// 解法精讲｜树上任意路径 DP：全局双臂、向上单臂
// - 核心要点：
//   1. 思路起点：递归返回从当前节点向下选择一条分支的最大贡献；当前节点可同时连接左右正贡献形成完整路径，并更新全局答案。
//   2. 执行逻辑：1. 空节点贡献 0；2. 左右贡献若为负则截断为 0；3. 用 node+left+right 更新答案，向父返回 node+max(left,right)。
//   3. 为什么这样做：任意非空简单路径有唯一最高节点；在该节点会组合其左右最佳向下贡献。向父传播时只能选择一侧，否则路径会分叉。
// - 边界与易错点：全局答案不能初始化为 0，否则全负树错误；路径不要求经过根；一个节点最多出现一次。
// - 举一反三：直径、最大路径和、最长同值路径都共享“节点处可闭合两臂，父方向只能接一臂”的框架。
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

