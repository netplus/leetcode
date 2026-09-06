// ============================================================================
// LC-94：二叉树的中序遍历
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个二叉树的根节点 root，返回它的中序遍历。
//
// 题目示例：
// 示例 1：
//   Input: root = [1,null,2,3]
//
//   Output: [1,3,2]
//
//   Explanation:
//
// 示例 2：
//   Input: root = [1,2,3,4,5,null,8,null,null,6,7,9]
//
//   Output: [4,2,6,5,7,1,3,9,8]
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
//   中序遍历的定义先于实现：
//
//         2
//        /     1   3
//
//   先完整访问左子树 -> 再访问根 2 -> 再访问右子树，结果 [1,2,3]。
//
//   站在节点 2 时，访问根之前必须先把‘以后还要回来处理 2’这件事记住。
//
// 2. 最自然的一般性解法
//   最自然的写法是递归：对当前节点先处理左子树，左边完成后记录当前节点，再处理右子树。这个顺序直接来自中序遍历定义。
//
// 3. 一般解法的问题与限制
//   递归会把‘处理完左边以后回到哪个祖先’保存在语言调用栈中。若题目要求手写迭代，就必须显式保存这些暂停现场；这不是新的遍历逻辑，只是把隐式状态拿出来。
//
// 4. 从具体问题抽象规律
//   任何递归树遍历都可以从‘函数暂停时必须记住什么’反推出显式状态结构。中序需要保存沿左链尚未访问根的祖先。
//
// 5. 专项算法｜中序遍历：栈保存“左子树回来后还要处理的祖先”
//   前面的推理已经得到可复用机制；现在才给它一个检索名称：树遍历：递归现场与显式栈。通用机制见 docs/algorithms/tree-traversal.md。回到本题时，只需要把其中的状态、边界和更新事件映射到当前题意，不要反过来用模板猜题。
//
// 6. 核心算法
//   一路向左时把祖先现场压栈；走不动时弹出最近祖先处理根，再转入它的右子树。
//
// 7. 公式 / 不变量
//   递归中序：
//   inorder(root->left)
//   visit(root)
//   inorder(root->right)
//
//   迭代不变量：
//   ancestors 中的每个节点都满足：
//   - 自己还没 visit；
//   - 正在或即将处理它的左子树。
//
//   当 current == nullptr：
//   栈顶节点的左子树已经完整结束，
//   因此此刻恰好是 visit(root) 的时间。
//
// 8. 执行步骤
//   1. current 从 root 开始；只要 current 非空，就压入 ancestors 并继续 current=current->left
//   2. 走到空节点后，弹出最近祖先；它的左子树已经完成，因此此时记录它的值
//   3. 令 current 指向该节点的右孩子，相当于开始递归处理右子树
//   4. 只要 current 非空或 ancestors 非空就继续，直到没有当前子树也没有待恢复祖先
//   记忆：左走时存现场，走到底后“弹一个、访问根、转右边”。
//
// 9. 为什么不会漏 / 不会重
//   一个节点第一次被 current 指到时只压栈一次；只有当它的整条左侧递归已经走到 null 后才会被弹出，因此左一定先于根。弹出后 current 转向右孩子，右子树又按同一规则完整处理，因此根一定先于右。每个节点压一次、弹一次，既不会遗漏也不会重复。
//
// 10. 边界与易错点
//   外层条件必须是 current || !stack.empty()；否则 current 变成 null 时会提前结束，来不及恢复祖先。若在压栈时记录值，就已经变成前序。不要把栈简单理解成“待访问节点”，中序里它更准确地表示未完成的递归现场。
//
// 11. 举一反三
//   LC-144 的栈保存“未来子树”，LC-94 的栈保存“暂停的祖先现场”。这个区别会直接迁移到 BST：中序会产生严格递增序列，因此 Day 11 的 LC-98 验证 BST 可以复用本题的访问顺序。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的层序节点值（-1 表示空节点）。
//   输出：中序遍历结果，以空格分隔。
// test.in 的预期输出：1 3 2
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
    vector<int> inorderTraversal(TreeNode* root) {
        vector<int> order;

        // ancestors 不是普通“待访问节点”集合，而是一路向左递归时被暂停的调用帧：
        // 栈中节点都还没访问自己，因为它们的左子树尚未完整返回。
        stack<TreeNode*> ancestors;
        TreeNode* current = root;

        // current 表示当前正在进入的子树；ancestors 非空表示仍有暂停祖先等待恢复。
        // 两者都为空时，整棵树才真正处理完。
        while (current || !ancestors.empty()) {
            // 模拟递归的 inorder(node->left)：一路向左下潜，
            // 同时把“左子树回来后还要访问根、再去右边”的祖先现场保存到栈里。
            while (current) {
                ancestors.push(current);
                current = current->left;
            }

            // current==nullptr 说明最近栈顶节点的左子树已经完整结束；
            // 此刻正好处于递归中的 Left 与 Right 之间，因此现在才访问这个根。
            current = ancestors.top();
            ancestors.pop();
            order.push_back(current->val);

            // 根访问完后开始 inorder(root->right)；右子树内部又会先一路向左下潜。
            current = current->right;
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
    auto ans = sol.inorderTraversal(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
