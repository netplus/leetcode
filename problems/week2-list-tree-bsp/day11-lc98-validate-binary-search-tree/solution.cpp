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
// 题目示例：
// 示例 1：
//   Input: root = [2,1,3]
//   Output: true
//
// 示例 2：
//   Input: root = [5,1,4,null,null,3,6]
//   Output: false
//   Explanation: The root node's value is 5 but its right child's value is 4.
//
// 约束与要求：
//   - 树中节点数目范围在 [1, 10^4] 内
//   - -2^31 <= Node.val <= 2^31 - 1
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜BST 全局约束下传：每深入一层，就把合法值域收紧成更小的开区间
//
// 前置概念
//   BST 的约束是“整棵子树”的全局约束，不是只要求每个父节点和它的两个直接孩子局部有序。
//
//   最容易产生的错误简化是递归检查：
//   `node->left->val < node->val < node->right->val`。
//   看下面这棵树：
//
//           5
//          / \
//         1   7
//            / \
//           4   8
//
//   如果只检查直接父子：
//   1 < 5 < 7   -> 通过
//   4 < 7 < 8   -> 通过
//   看起来每个局部节点都合法。
//
//   但节点 4 位于根 5 的整个右子树中，所以它还必须满足 4 > 5；这一条祖先约束被局部父子比较完全漏掉了。
//   因此 BST 判断必须让深层节点继续携带祖先留下的约束。上下界递归把这些约束压成 `(lower, upper)`；中序遍历严格递增则是另一种等价的全局验证方式。
//
// 0. 优化是怎么来的
//   一个很自然且容易证明正确的办法，是先做一次中序遍历，把所有值保存到数组；BST 当且仅当这个中序数组严格递增。时间 O(n)，但需要 O(n) 额外结果空间。
//
//   如果不想保存整条中序序列，可以把“已经看到的全局约束”直接携带到递归里。每经过一个祖先，当前子树允许的值域都会被进一步收紧：进入左子树增加一个严格上界，进入右子树增加一个严格下界。
//
//   这样不再事后生成序列再验证，而是在节点第一次被访问时立即用祖先累计出的 (lower, upper) 判断是否合法；递归栈本身就是唯一需要的路径状态。
//
//   优化类型：完整遍历结果缓冲 -> 把必要的历史约束压缩成两个边界参数；额外状态从 O(n) 序列降为 O(h) 递归栈。
//
// 1. 图像直觉
//   最容易犯的错误是只检查“左孩子 < 根 < 右孩子”。
//   看这棵树：
//
//           5
//          / \
//         1   7
//            / \
//           4   8
//
//   局部看：
//   4 < 7，看起来像合法左孩子。
//   但 4 位于根 5 的右子树，必须满足“整个右子树都 > 5”，所以它其实非法。
//
//   因此节点 4 不是只带着父亲 7 的要求走下来，而是带着祖先一路传下来的区间：
//   root 5:     (-∞, +∞)
//   right 7:   (5, +∞)
//   left 4:    (5, 7)
//
//   4 不满足 5 < 4 < 7，于是立刻判 false。
//
//   可以把每个节点想成进入一扇越来越窄的门：祖先给出的上下界必须全部满足。
//
// 2. 一句话核心
//   不要只和父节点比较；递归携带当前子树允许的开区间 (lower, upper)，每个节点必须同时满足所有祖先留下的边界。
//
// 3. 公式 / 不变量
//   validate(node, lower, upper) 的不变量：
//   当前 node 以及它整棵子树都处在祖先已经确定的合法上下文中，node 必须满足：
//
//   lower < node->val < upper
//
//   进入左子树：
//   validate(node->left, lower, node->val)
//   因为左边所有值都必须 < node->val
//
//   进入右子树：
//   validate(node->right, node->val, upper)
//   因为右边所有值都必须 > node->val
//
//   根节点初始范围：
//   (LLONG_MIN, LLONG_MAX)
//
// 4. 执行步骤
//   1. 根节点从一个覆盖所有 int 值的开区间 (LLONG_MIN, LLONG_MAX) 开始
//   2. 空节点天然合法；非空节点先检查 lower < val < upper，不满足立即返回 false
//   3. 递归验证左子树，并把当前 val 作为新的严格上界
//   4. 递归验证右子树，并把当前 val 作为新的严格下界；左右都成立才返回 true
//   记忆：不是问“我比爸爸大还是小”，而是问“我是否还在所有祖先共同留下的合法区间里”。
//
// 5. 为什么不会漏 / 不会重
//   若递归区间始终正确，那么左子树继承所有旧祖先限制并新增 val 这个上界，右子树同理新增下界，因此任意节点通过检查当且仅当它满足从根到自己的全部 BST 祖先约束。反过来，一棵合法 BST 的每个节点显然都位于这样逐层收紧得到的区间中，所以不会误杀。每个节点只检查一次，不会漏重。
//
// 6. 边界与易错点
//   BST 使用严格不等号，重复值非法。不能用 INT_MIN/INT_MAX 直接充当开区间哨兵，因为节点值本身允许等于 int 极值；用 long long 边界才能把所有合法 int 值包含进去。只比较直接左右孩子会漏掉跨层违规。
//
// 7. 举一反三
//   Day 10 LC-94 给出另一条等价视角：合法 BST 的中序序列必须严格递增。本题选择上下界法，是为了训练“把祖先上下文作为递归参数下传”；这种模型会迁移到范围约束、表达式作用域和路径条件验证。
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
    // validate(node, lower, upper) 的契约：当前 node 必须落在所有祖先共同留下的严格开区间中，
    // 且整棵子树也要继续满足由这个上下文递推出来的 BST 约束。
    bool validate(TreeNode* node, long long lower, long long upper) {
        // 空子树没有任何节点违反约束，因此天然合法。
        if (!node) return true;

        // BST 使用严格不等号；等于任一边界也非法，所以重复值会被拒绝。
        // 这里检查的是祖先累计出的全局区间，而不是只和直接父节点比较。
        if (node->val <= lower || node->val >= upper) return false;

        // 左子树继承旧 lower，并把当前值收紧为新的严格 upper；
        // 右子树继承旧 upper，并把当前值收紧为新的严格 lower。
        return validate(node->left, lower, node->val)
            && validate(node->right, node->val, upper);
    }

public:
    bool isValidBST(TreeNode* root) {
        // 节点值本身允许等于 INT_MIN/INT_MAX，不能用 int 极值当开区间哨兵；
        // 用 long long 的更宽边界才能把所有合法 int 值完整包含进去。
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
