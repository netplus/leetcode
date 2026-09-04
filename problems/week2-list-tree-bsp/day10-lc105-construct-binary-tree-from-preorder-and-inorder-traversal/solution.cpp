// ============================================================================
// LC-105：从前序与中序遍历序列构造二叉树
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定两个整数数组 preorder 和 inorder，其中 preorder 是二叉树的先序遍历，inorder 是同一棵树的中序遍历，请构造二叉树并返回其根节点。
//
// 约束与要求：
//   - 1 <= preorder.length <= 3000
//   - inorder.length == preorder.length
//   - -3000 <= preorder[i], inorder[i] <= 3000
//   - preorder 和 inorder 均无重复元素
//   - inorder 均出现在 preorder
//   - preorder 保证为二叉树的前序遍历序列
//   - inorder 保证为二叉树的中序遍历序列
//
// 复杂度目标：O(n) 时间（使用哈希表）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜前序定根 + 中序切左右：用两个遍历序列给递归子树划边界
//
// 0. 优化是怎么来的
//   递归结构本身很自然：preorder 当前第一个未消费值确定根，然后在当前 inorder 区间里找到这个根的位置，再切成左右子树。
//
//   如果每层递归都在线性扫描 inorder 寻找 rootValue，这个算法仍然正确，但极端链状树会产生 n+(n-1)+...+1 次查找，最坏 O(n^2)。
//
//   题目保证节点值唯一，因此 rootValue 在 inorder 中的位置可以提前建立成 value -> index。每个递归层的“重新线性找根”就变成一次 O(1) 查询，而每个节点只消费一次。
//
//   优化类型：递归中的重复查找 -> 预建 inorder 索引。整体从最坏 O(n^2) 降到 O(n)。
//
// 1. 图像直觉
//   用经典例子：
//   preorder = [3, 9, 20, 15, 7]
//   inorder  = [9, 3, 15, 20, 7]
//
//   前序第一项一定是当前根：3
//   在中序里找到 3：
//
//   inorder : [9] | 3 | [15,20,7]
//              左       右
//
//   于是根的左右节点集合已经确定。
//   前序又保证“根后面先完整出现左子树，再出现右子树”：
//   preorder: 3 | [9] | [20,15,7]
//
//   对子树重复同一个动作：
//   当前前序首元素定根，中序位置切左右。
//
//   哈希表 value -> inorder index 只是为了 O(1) 找切口；真正决定结构的是两个遍历顺序提供的互补信息。
//
// 2. 一句话核心
//   前序告诉我们“下一个根是谁”，中序告诉我们“这个根把当前节点集合切成哪两棵子树”；递归不断重复这两个信息。
//
// 3. 公式 / 不变量
//   当前递归负责 inorder[left..right]。
//   若 left > right：空子树。
//
//   rootValue = preorder[preorderIndex++]
//   middle = inorderIndex[rootValue]
//
//   左子树节点集合：inorder[left .. middle-1]
//   右子树节点集合：inorder[middle+1 .. right]
//
//   因为 preorder 顺序是 Root, Left, Right：
//   消费完 root 后必须先递归 build(left)，再 build(right)。
//
//   节点值唯一 => middle 唯一 => 每次切分唯一。
//
// 4. 执行步骤
//   1. 预处理 inorder 的 value->index 哈希表，把每次寻找根位置从 O(n) 降为 O(1)
//   2. build(left,right) 若区间为空直接返回 nullptr；否则从 preorderIndex 取当前根并推进游标
//   3. 用根值在 inorder 中的位置 middle 把当前合法区间切成左区间和右区间
//   4. 严格先递归构建左子树，再递归构建右子树，因为 preorder 在根之后就是先左后右
//   5. 顶层调用覆盖整个 inorder 区间，递归完成后返回整棵树根节点
//   记忆：前序负责点名根，中序负责划地盘；根后先建左，再建右。
//
// 5. 为什么不会漏 / 不会重
//   节点值唯一时，当前根在 inorder 中只有一个位置，因此左右子树的节点集合唯一。preorder 的第一个未消费元素必是当前子树根，而其后紧接着的是完整左子树的前序，再是右子树前序；递归按照 inorder 区间限制消费范围，左子树恰好消费属于左区间的那些根，随后游标自然来到右子树。每个 preorder 元素只消费一次，每个 inorder 节点只作为一次切点，所以不漏不重，结构也唯一。
//
// 6. 边界与易错点
//   必须先建 left 再建 right；若交换顺序，全局 preorderIndex 会被错误子树先消费。inorderIndex 和 preorderIndex 是成员状态，本实现每次 buildTree 前都 clear/reset，避免同一个 Solution 对象重复调用时污染。该 O(n) 方法依赖节点值唯一；有重复值时 value->index 不再唯一。
//
// 7. 举一反三
//   这题把 LC-144/94 的遍历顺序反过来使用：前面是“树 -> 序列”，这里是“两个序列 -> 树”。若给 inorder + postorder，则后序末尾定根，但因为从后往前消费顺序是 Root, Right, Left，所以递归顺序也要反过来先建右子树。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个前序遍历值。
//   第 3 行：n 个中序遍历值。
//   输出：构造后二叉树的层序序列（-1 表示空节点）。
// test.in 的预期输出：3 9 20 -1 -1 15 7
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
    unordered_map<int, int> inorderIndex;
    int preorderIndex = 0;

    TreeNode* build(const vector<int>& preorder, int left, int right) {
        if (left > right) return nullptr;
        const int rootValue = preorder[preorderIndex++];
        TreeNode* root = new TreeNode(rootValue);
        const int middle = inorderIndex[rootValue];
        root->left = build(preorder, left, middle - 1);
        root->right = build(preorder, middle + 1, right);
        return root;
    }

public:
    TreeNode* buildTree(vector<int>& preorder, vector<int>& inorder) {
        inorderIndex.clear();
        preorderIndex = 0;
        for (int i = 0; i < static_cast<int>(inorder.size()); ++i) {
            inorderIndex[inorder[i]] = i;
        }
        return build(preorder, 0, static_cast<int>(inorder.size()) - 1);
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> pre(n), in(n);
    for (int i = 0; i < n; ++i) cin >> pre[i];
    for (int i = 0; i < n; ++i) cin >> in[i];
    TreeNode* root = sol.buildTree(pre, in);
    printTree(root);
    return 0;
}
