// ============================================================================
// LC-144：二叉树的前序遍历
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你二叉树的根节点 root，返回它节点值的前序遍历。
//
// 题目示例：
// 示例 1：
//   Input: root = [1,null,2,3]
//
//   Output: [1,2,3]
//
//   Explanation:
//
// 示例 2：
//   Input: root = [1,2,3,4,5,null,8,null,null,6,7,9]
//
//   Output: [1,2,4,5,6,7,3,8,9]
//
//   Explanation:
//
// 示例 3：
//   Input: root = []
//
//   Output: []
//
// 示例 4：
//   Input: root = [1]
//
//   Output: [1]
//
// 约束与要求：
//   - 树中节点数目在范围 [0, 100] 内
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜从题目直觉到可复用算法
//
// 1. 题目直觉 / 图形模型
//   前序的规则就是：
//
//           1
//          / ╲
//         2   3
//
//   先访问根 1，再完整访问左子树 2，最后右子树 3。
//
//   如果不用递归，访问 1 后必须记住‘2 应先处理、3 稍后处理’。
//
// 2. 最自然的一般性解法
//   最自然的实现是递归：记录当前节点，然后递归左子树，再递归右子树。它与前序定义一一对应。
//
// 3. 一般解法的问题与限制
//   递归把尚未处理的右子树和返回位置隐藏在调用栈里。改成迭代时，需要显式保存未来待处理节点；因为后进先出，想先处理左边，就必须先保存右边、再保存左边。
//
// 4. 从具体问题抽象规律
//   迭代树遍历的关键不是记压栈顺序，而是先写目标访问顺序，再根据‘后保存的先处理’反推待办状态如何放入容器。
//
// 5. 专项算法｜前序遍历：根先处理，栈只负责保存之后要走的分支
//   前面的推理已经得到可复用机制；现在才给它一个检索名称：树的前序遍历与显式递归现场。通用机制见 docs/algorithms/tree-traversal.md。回到本题时，只需要把其中的状态、边界和更新事件映射到当前题意，不要反过来用模板猜题。
//
// 6. 核心算法
//   前序遍历的本质是“节点一到手就处理”；栈保存尚未进入的子树，并用逆序压栈保证下一次先进入左子树。
//
// 7. 公式 / 不变量
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
// 8. 执行步骤
//   1. 空树直接返回；否则把 root 作为第一个待进入节点压栈
//   2. 每次弹出栈顶 node，因为它就是当前前序中的下一个节点，所以立即记录 node->val
//   3. 若右孩子存在先压右孩子，若左孩子存在再压左孩子，使左孩子位于栈顶
//   4. 重复直到栈为空，此时所有节点都已经恰好进入一次
//   记忆：前序是“拿到节点就处理”；想先走谁，就让谁最后入栈。
//
// 9. 为什么不会漏 / 不会重
//   每个非空节点只会由它唯一的父节点压栈一次（根单独压一次），因此不会重复。弹出一个节点时，右子树虽先入栈，却被左子树压在下面；左子树内部也遵守同样规则，所以整棵左子树一定先于右子树完成。于是根、左、右的相对次序在每棵子树上都成立，也不会漏掉任何非空孩子。
//
// 10. 边界与易错点
//   最常见错误是把左右孩子的压栈顺序写成 left 再 right，这会得到根-右-左。还要区分“压栈时机”和“访问时机”：本题是在 pop 后立即记录值。空节点不必压栈，可减少状态分支。
//
// 11. 举一反三
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

        // pending 保存“以后还要进入的子树根”；栈顶始终是按前序顺序下一个该处理的节点。
        stack<TreeNode*> pending;
        pending.push(root);

        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();

            // 前序是 Root -> Left -> Right，所以节点一旦从栈顶成为当前节点，就立即访问根值。
            order.push_back(node->val);

            // 栈后进先出，而左子树必须先于右子树处理：
            // 因此反着保存未来分支——先压右，再压左，让 left 成为新的栈顶。
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
