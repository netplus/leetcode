// ============================================================================
// LC-236：二叉树的最近公共祖先
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 11 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个二叉树，找到该树中两个指定节点的最近公共祖先。
// 最近公共祖先是满足以下条件的节点 x：x 是 p、q 的祖先，并且 x 的深度尽可能大；一个节点也可以是它自己的祖先。
//
// 题目示例：
// 示例 1：
//   Input: root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 1
//   Output: 3
//   Explanation: The LCA of nodes 5 and 1 is 3.
//
// 示例 2：
//   Input: root = [3,5,1,6,2,0,8,null,null,7,4], p = 5, q = 4
//   Output: 5
//   Explanation: The LCA of nodes 5 and 4 is 5, since a node can be a descendant of itself according to the LCA definition.
//
// 示例 3：
//   Input: root = [1,2], p = 1, q = 2
//   Output: 1
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
// 解法精讲｜后序证据汇聚：子树向上报告 p/q，第一次左右同时有证据的位置就是 LCA
//
// 1. 图像直觉
//   把递归返回值看成“这棵子树交上来的证据”，而不是普通节点查找。
//
//   例如：
//             3
//            / \
//           5   1
//          / \ / \
//         6  2 0  8
//           / \
//          7   4
//
//   若 p=5，q=4：
//   - 走到 5 时，当前节点自己就是 p，于是直接把 5 作为证据向上交；
//   - 虽然 4 在 5 的子树里，但题目允许“节点是自己的祖先”，因此 5 就已经是答案。
//
//   若 p=6，q=4：
//   在节点 5：
//   left  子树交回 6
//   right 子树交回 4
//   两边同时非空 -> 两条目标路径第一次在 5 汇合 -> LCA=5。
//
//   图像上就是：从 p、q 各往根画一条线，最低的交汇点就是答案。
//
// 2. 一句话核心
//   递归返回“当前子树里找到的目标证据”；若左右子树各交回一份证据，当前节点就是两条路径最低的汇合点。
//
// 3. 公式 / 不变量
//   定义 L(node) 的返回语义：
//   - nullptr：这棵子树没有证据；
//   - p 或 q：当前节点本身就是目标，直接上报；
//   - 其他非空节点：代表这棵子树已经确定出的 LCA / 唯一目标证据。
//
//   left  = L(node->left)
//   right = L(node->right)
//
//   left != nullptr && right != nullptr  -> return node
//   只有一边非空                         -> return 非空那边
//   两边都空                             -> return nullptr
//
//   题目保证 p、q 都存在，所以根最终一定能得到答案。
//
// 4. 执行步骤
//   1. 遇到空节点返回 nullptr；若当前节点就是 p 或 q，直接返回当前节点作为证据
//   2. 后序递归获取左子树证据 left 和右子树证据 right
//   3. 若 left、right 都非空，说明 p/q 分居两侧，当前 root 就是最低汇合点，返回 root
//   4. 否则把唯一非空证据继续向父节点上报；两边都空则自然返回 nullptr
//   记忆：左右都报到 -> 我就是汇合点；只报一边 -> 原样向上传。
//
// 5. 为什么不会漏 / 不会重
//   若 p、q 分居当前节点两侧，任何更低节点都不可能同时包含两者，因此当前节点就是最低公共祖先。若两者都在同一侧，另一侧返回空，答案已由包含两者的那棵子树递归确定并向上传递。若当前节点本身是 p 或 q，因为节点可以是自己的祖先，直接上报不会错。每个节点只访问一次，所以无遗漏、无重复搜索。
//
// 6. 边界与易错点
//   这里比较的是节点身份 root==p/root==q，而不是只比较 val；虽然本题值唯一，但 LeetCode 方法签名语义仍是节点。最容易误解的是“命中 p/q 就提前返回”：这并不会漏掉其后代中的另一个目标，因为如果另一个目标在其子树中，当前目标本身就应当是 LCA。
//
// 7. 举一反三
//   这是一类非常通用的后序汇聚：子树返回“证据”，父节点在证据首次同时出现时完成答案。Day 14 LC-124 也会用后序返回一条可向上的信息，同时在当前节点闭合一个全局候选答案。
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
        // 返回值不是普通“搜索结果”，而是当前子树向父节点上报的一份有效证据：
        // nullptr 表示这棵子树没有目标；p/q 表示当前命中目标；其它非空节点表示子树内已形成汇合点。
        // 当前节点若就是 p/q，可直接上报自己；若另一目标在它的后代中，按定义它本身就应是 LCA。
        if (!root || root == p || root == q) return root;

        // 后序收集左右子树证据；每棵子树内部会先完成自己的判断再向当前节点汇报。
        TreeNode* left = lowestCommonAncestor(root->left, p, q);
        TreeNode* right = lowestCommonAncestor(root->right, p, q);

        // 左右同时非空，说明两份目标证据第一次在当前节点的两侧汇合；
        // 更低的单个子树不可能同时包含两边证据，因此当前 root 就是最低公共祖先。
        if (left && right) return root;

        // 只有一边有证据时，当前节点还不是汇合点，原样把唯一证据继续向父节点传递；
        // 两边都空时这个表达式也自然返回 nullptr。
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
