// ============================================================================
// LC-145：二叉树的后序遍历
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一棵二叉树的根节点 root，返回其节点值的后序遍历。
//
// 约束与要求：
//   - 树中节点的数目在范围 [0, 100] 内
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜后序遍历：先构造根-右-左，再整体反转成左-右-根
//
// 1. 图像直觉
//   后序想要：左 -> 右 -> 根。
//   单栈直接模拟时，节点必须等左右子树都结束后才能访问，状态比前序复杂。
//   本实现换一个角度：先做一个容易生成的镜像前序。
//
//   目标：      左  右  根
//   整体反转：  根  右  左
//
//   所以先生成 根 -> 右 -> 左，再 reverse。
//
//   例如：
//           1
//          / \
//         2   3
//
//   先得到 [1,3,2]
//   reverse -> [2,3,1]
//   正好是后序。
//
// 2. 一句话核心
//   把难直接生成的“左-右-根”翻转成容易用栈生成的“根-右-左”，最后一次 reverse 恢复目标顺序。
//
// 3. 公式 / 不变量
//   postorder = Left + Right + Root
//   reverse(postorder)
//             = Root + reverse(Right) + reverse(Left)
//
//   对每棵子树都按“根、右、左”记录，
//   最终把整个序列 reverse，
//   就得到“左、右、根”。
//
//   为了弹出时先得到右孩子：
//   push(left)
//   push(right)
//   因为 right 后入先出。
//
// 4. 执行步骤
//   1. 空树返回空结果；否则 root 入栈
//   2. 每次弹出 node 并立即记录，当前正在生成的是根-右-左序列
//   3. 先压左孩子、再压右孩子，使右孩子下一次先弹出
//   4. 所有节点记录完后 reverse(order)，得到真正的左-右-根后序
//   记忆：后序不好直接做，就先做“根右左”，最后反转。
//
// 5. 为什么不会漏 / 不会重
//   第一次扫描中，每个节点仍只入栈和出栈一次，不会漏重。对任意父节点，生成序列中它在两个子树之前，且右子树整体在左子树之前；整体反转后，相对顺序同步翻转为左子树整体、右子树整体、父节点。因此每个局部子树都满足后序，整棵树也满足。
//
// 6. 边界与易错点
//   这里的压栈顺序与 LC-144 正好相反：为了生成根-右-左，要先 push(left) 再 push(right)。reverse 的是结果数组，不是树。该方案需要 O(n) 结果空间本来就不可避免；如果题目要求流式输出，才需要 visited/lastVisited 的真正单栈后序。
//
// 7. 举一反三
//   这是典型的“目标顺序难直接维护 -> 构造一个容易的逆序 -> 最后统一变换”。和 Day 7 LC-31 的思路一样，都不是机械模拟目标，而是寻找更容易维护的等价表示。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的层序节点值（-1 表示空节点）。
//   输出：后序遍历结果，以空格分隔。
// test.in 的预期输出：3 2 1
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
    vector<int> postorderTraversal(TreeNode* root) {
        vector<int> order;
        if (!root) return order;
        stack<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            TreeNode* node = pending.top();
            pending.pop();
            order.push_back(node->val);
            if (node->left) pending.push(node->left);
            if (node->right) pending.push(node->right);
        }
        reverse(order.begin(), order.end());
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
    auto ans = sol.postorderTraversal(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
