// ============================================================================
// LC-98：验证二叉搜索树
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 11 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个二叉树的根节点 root，判断其是否是一个有效的二叉搜索树。
// 有效二叉搜索树定义如下：节点的左子树只包含严格小于当前节点的数。
// 节点的右子树只包含严格大于当前节点的数。
// 所有左子树和右子树自身必须也是二叉搜索树。
//
// 约束与要求：
//   - 树中节点数目范围在 [1, 10^4] 内
//   - -2^31 <= Node.val <= 2^31 - 1
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜递归上下界：验证整棵子树的取值范围
// - 核心要点：
//   1. 思路起点：每个节点不仅要和父节点比较，还必须满足所有祖先传下来的开区间 (lower,upper)。
//   2. 执行逻辑：1. 根从无限上下界开始；2. 检查 lower<value<upper；3. 左子树收紧上界为 value，右子树收紧下界。
//   3. 为什么这样做：祖先限制随递归完整传递，节点通过检查当且仅当它满足路径上所有 BST 约束；左右子树递归成立则整棵树成立。
// - 边界与易错点：BST 是严格不等，重复值非法；int 极值不能用 int 哨兵，使用 long long；只比较直接孩子会漏掉跨层违例。
// - 举一反三：把全局约束沿树边逐步收紧，适用于范围树、表达式合法性和带上下文的递归验证。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个层序节点值（-1 表示空节点）。
//   输出：是有效二叉搜索树时输出 1，否则输出 0。
// test.in 的预期输出：0
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
    bool validate(TreeNode* node, long long lower, long long upper) {
        if (!node) return true;
        if (node->val <= lower || node->val >= upper) return false;
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }

public:
    bool isValidBST(TreeNode* root) {
        return validate(root, LLONG_MIN, LLONG_MAX);
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
    cout << (sol.isValidBST(buildTreeLO(a)) ? 1 : 0) << "\n";
    return 0;
}

