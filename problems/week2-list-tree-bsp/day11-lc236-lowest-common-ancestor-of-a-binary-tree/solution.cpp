// ============================================================================
// LC-236：二叉树的最近公共祖先
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 11 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个二叉树, 找到该树中两个指定节点的最近公共祖先。
// 百度百科中最近公共祖先的定义为：“对于有根树 T 的两个节点 p、q，最近公共祖先表示为一个节点 x，满足 x 是 p、q 的祖先且 x 的深度尽可能大（一个节点也可以是它自己的祖先）。
// ”
//
// 约束与要求：
//   - 树中节点数目在范围 [2, 10^5] 内。
//   - -10^9 <= Node.val <= 10^9
//   - 所有 Node.val 互不相同。
//   - p != q
//   - p 和 q 均存在于给定的二叉树中。
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜后序 DFS：让命中信息向上汇聚
// - 核心要点：
//   1. 思路起点：递归返回值表示当前子树是否找到 p/q，以及能代表该命中的节点；若左右两侧都非空，当前根就是最近公共祖先。
//   2. 执行逻辑：1. 空节点或当前命中 p/q 时直接返回；2. 递归查询左右子树；3. 两边都命中返回 root，否则返回非空一边。
//   3. 为什么这样做：若 p/q 分居左右，当前节点是最低的汇合点；若同在一侧，该侧递归已返回其最近公共祖先；若当前就是其中一个，它作为祖先直接上报。
// - 边界与易错点：比较节点地址而非值；题目保证 p、q 都存在且不同；函数返回的是节点指针。
// - 举一反三：“子树返回证据，首次汇合处作决策”也用于目录公共祖先、组织树权限汇聚和树上目标集合覆盖。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个层序节点值（-1 表示空节点）。
//   第 3 行：p、q（两个节点的值）。
//   输出：最近公共祖先节点的值。
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
static TreeNode* findNode(TreeNode* root, int val) {
    if (!root) return nullptr;
    if (root->val == val) return root;
    TreeNode* l = findNode(root->left, val);
    if (l) return l;
    return findNode(root->right, val);
}

// ---------- 题解实现 ----------
class Solution {
public:
    TreeNode* lowestCommonAncestor(TreeNode* root, TreeNode* p, TreeNode* q) {
        if (!root || root == p || root == q) return root;
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);
        if (left && right) return root;
        return left ? left : right;
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
    int pv, qv; cin >> pv >> qv;
    TreeNode* root = buildTreeLO(a);
    TreeNode* p = findNode(root, pv);
    TreeNode* q = findNode(root, qv);
    TreeNode* ans = sol.lowestCommonAncestor(root, p, q);
    cout << (ans ? ans->val : -1) << "\n";
    return 0;
}

