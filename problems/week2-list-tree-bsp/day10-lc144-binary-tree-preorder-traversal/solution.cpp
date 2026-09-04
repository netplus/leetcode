// ============================================================================
// LC-144：二叉树的前序遍历
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你二叉树的根节点 root，返回它节点值的前序遍历。
//
// 约束与要求：
//   - 树中节点数目在范围 [0, 100] 内
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜前序遍历：根先处理，栈只负责保存之后要走的分支
//
// 1. 图像直觉
//   先把递归的动作画出来，而不是先背“右压栈、左压栈”：
//
//           1
//          / \
//         2   3
//        / \
//       4   5
//
//   前序 = 根 -> 左 -> 右
//   所以从 1 出发时：
//   1 已经立刻输出；接下来要先走 2，但 3 不能丢。
//
//   显式栈就是“以后再回来走的分支”：
//   输出 1 后，栈里要让 2 位于顶部。
//   stack(top -> bottom): 2, 3
//
//   由于栈后进先出，代码必须先 push(3)，再 push(2)。
//
// 2. 一句话核心
//   前序遍历的本质是“节点一到手就处理”；栈保存尚未进入的子树，并用逆序压栈保证下一次先进入左子树。
//
// 3. 公式 / 不变量
//   递归前序的时间顺序：
//   visit(root)
//   preorder(root->left)
//   preorder(root->right)
//
//   迭代版维护不变量：
//   栈顶 = 按前序顺序下一个应该进入的节点。
//
//   因此弹出 node 后：
//   1. visit(node)
//   2. push(node->right)
//   3. push(node->left)
//
//   右先入栈只是因为它应该更晚被处理。
//
// 4. 执行步骤
//   1. 空树直接返回；否则把 root 作为第一个待进入节点压栈
//   2. 每次弹出栈顶 node，因为它就是当前前序中的下一个节点，所以立即记录 node->val
//   3. 若右孩子存在先压右孩子，若左孩子存在再压左孩子，使左孩子位于栈顶
//   4. 重复直到栈为空，此时所有节点都已经恰好进入一次
//   记忆：前序是“拿到节点就处理”；想先走谁，就让谁最后入栈。
//
// 5. 为什么不会漏 / 不会重
//   每个非空节点只会由它唯一的父节点压栈一次（根单独压一次），因此不会重复。弹出一个节点时，右子树虽先入栈，却被左子树压在下面；左子树内部也遵守同样规则，所以整棵左子树一定先于右子树完成。于是根、左、右的相对次序在每棵子树上都成立，也不会漏掉任何非空孩子。
//
// 6. 边界与易错点
//   最常见错误是把左右孩子的压栈顺序写成 left 再 right，这会得到根-右-左。还要区分“压栈时机”和“访问时机”：本题是在 pop 后立即记录值。空节点不必压栈，可减少状态分支。
//
// 7. 举一反三
//   Day 10 的 LC-94/145 只是在改变“什么时候处理根”。前序是进入节点时处理；中序要等左子树回来；后序要等左右都回来。后续树题应优先问：递归函数在进入、左右子树之间、还是退出节点时做决策？
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的层序节点值（-1 表示空节点）。
//   输出：前序遍历结果，以空格分隔。
// test.in 的预期输出：1 2 3
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
    vector<int> preorderTraversal(TreeNode* root) {
        vector<int> order;
        if (!root) return order;
        stack<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();
            order.push_back(node->val);
            if (node->right) pending.push(node->right);
            if (node->left) pending.push(node->left);
        }
        return order;
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
    auto ans = sol.preorderTraversal(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
