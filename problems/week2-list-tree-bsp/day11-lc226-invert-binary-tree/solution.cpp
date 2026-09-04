// ============================================================================
// LC-226：翻转二叉树
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 11 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一棵二叉树的根节点 root，翻转这棵二叉树，并返回其根节点。
//
// 约束与要求：
//   - 树中节点数目范围在 [0, 100] 内
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜树结构变换：先让左右子树完成职责，再交换当前节点的两个结果
//
// 1. 图像直觉
//   不要先把它理解成“每个节点 swap 一下”，先看递归职责：
//
//   原树：                 镜像后：
//           4                       4
//          / \                     / \
//         2   7                   7   2
//        / \ / \                 / \ / \
//       1  3 6  9               9  6 3  1
//
//   站在节点 4，只需要提出两个子问题：
//   - 左边这棵 2 子树，请你自己完整镜像；
//   - 右边这棵 7 子树，请你自己完整镜像。
//
//   等两边都交回“已经镜像好的子树”后，4 只做最后一步：
//   原右子树放到 left，原左子树放到 right。
//
//   所以当前节点并不负责整棵树的所有细节，它只负责组合两个已经正确的子问题结果。
//
// 2. 一句话核心
//   递归函数返回“已经翻转完成的这棵子树”；当前节点只需交换两个已完成的子树，再把自己作为新子树根返回。
//
// 3. 公式 / 不变量
//   定义 F(node)：返回以 node 为根、已经完全镜像后的子树根。
//
//   空树：
//   F(nullptr) = nullptr
//
//   非空节点：
//   left  = F(node->left)
//   right = F(node->right)
//   node->left  = right
//   node->right = left
//   return node
//
//   递归不变量：执行交换之前，left/right 已分别是原左右子树的完整镜像。
//
// 4. 执行步骤
//   1. 空节点没有结构可翻转，直接返回 nullptr
//   2. 递归翻转原左子树并保存返回根 left
//   3. 递归翻转原右子树并保存返回根 right
//   4. 把 right 接到 root->left、left 接到 root->right，然后返回 root
//   记忆：子树先各自照镜子，父节点最后把左右位置对调。
//
// 5. 为什么不会漏 / 不会重
//   对任意节点，假设它的左右子树递归返回时都已经是各自原结构的镜像。当前节点交换这两棵镜像子树的位置后，左右方向也被反转，于是以当前节点为根的整棵子树就是原子树的镜像。叶子和空树显然成立，因此按树高归纳整棵树成立。每个节点只被唯一一次递归访问，不会漏也不会重复。
//
// 6. 边界与易错点
//   如果直接写 root->left = invertTree(root->right) 后又基于已经改过的 root->left 去处理另一边，可能覆盖原指针；本实现先把两个递归结果保存到局部变量最清晰。题目要原地修改结构，无需新建节点。
//
// 7. 举一反三
//   这与 Day 10 LC-105“左右子树先递归构造，再挂回根”是同一种组合式递归。后续遇到树结构变换，优先定义：递归返回的是一棵什么样的“已经处理好的子树”？
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的层序节点值（-1 表示空节点）。
//   输出：翻转后二叉树的层序序列（-1 表示空节点）。
// test.in 的预期输出：4 7 2 9 6 3 1
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
    TreeNode* invertTree(TreeNode* root) {
        if (!root) return nullptr;
        TreeNode* left = invertTree(root->left);
        TreeNode* right = invertTree(root->right);
        root->left = right;
        root->right = left;
        return root;
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
    printTree(sol.invertTree(buildTreeLO(a)));
    return 0;
}
