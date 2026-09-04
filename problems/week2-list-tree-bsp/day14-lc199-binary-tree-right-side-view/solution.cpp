// ============================================================================
// LC-199：二叉树的右视图
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 14 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个二叉树的根节点 root，想象自己站在它的右侧，按照从顶部到底部的顺序，返回从右侧所能看到的节点值。
//
// 题目示例：
// 示例 1：
//   Input: root = [1,2,3,null,5,null,4]
//   Output: [1,3,4]
//   Explanation:
//
// 示例 2：
//   Input: root = [1,2,3,4,null,null,null,5]
//   Output: [1,3,4,5]
//   Explanation:
//
// 示例 3：
//   Input: root = [1,null,3]
//   Output: [1,3]
//
// 示例 4：
//   Input: root = []
//   Output: []
//
// 约束与要求：
//   - 二叉树的节点个数的范围是 [0,100]
//   - -100 <= Node.val <= 100
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜层序遍历的层内聚合：每层只取最右一个节点
//
// 1. 图像直觉
//   例如：
//
//           1
//          / \
//         2   3
//          \   \
//           5   4
//
//   从右侧看到：1, 3, 4。
//
//   BFS 分层：
//   level 0 queue: [1]
//                    ^ 最后一个也是 1
//
//   level 1 queue: [2, 3]
//                       ^ 该层最后一个 = 3
//
//   level 2 queue: [5, 4]
//                       ^ 该层最后一个 = 4
//
//   所以真正需要维护的不是“右侧视角的几何射线”，而是 LC-102 已经建立的层边界：
//
//   width = queue.size()
//
//   处理第 i 个节点时：
//   若 i == width-1
//   => 它就是当前层最后一个节点
//   => 记录答案。
//
// 2. 一句话核心
//   完全复用层序遍历：冻结当前层 width，按从左到右处理这一层，只在 i==width-1 时记录节点值。
//
// 3. 公式 / 不变量
//   每轮开始的不变量：
//   queue 中恰好是同一 depth 的全部节点，并按从左到右排列。
//
//   width = queue.size()
//
//   for i in [0, width):
//       pop 当前层第 i 个节点
//       if i == width-1:
//           answer.push_back(node->val)
//
//   因为孩子按 left 再 right 入队，下一轮 queue 仍保持下一层从左到右顺序。
//
//   注意 width 必须在处理这一层之前冻结；循环中 queue.size() 会因为加入孩子而变化。
//
// 4. 执行步骤
//   1. 空树直接返回空 answer；否则根节点入队，建立第 0 层 frontier
//   2. 每轮开始先保存 width=pending.size()，把它冻结为当前层节点数量
//   3. 循环 width 次从队首取节点；只有 i==width-1 时把 node->val 加入答案
//   4. 仍按 left、right 顺序把非空孩子加入队尾，让下一轮队列保持从左到右的层序顺序
//   5. 当前层恰好处理 width 个节点后进入下一轮，直到队列为空
//   记忆：LC-102 原样复用；一层只取最后一个。
//
// 5. 为什么不会漏 / 不会重
//   LC-102 已保证冻结 width 后，一轮恰好处理同一深度的全部节点；当前实现又按从左到右的顺序处理这一层，所以最后一个节点就是该深度最右节点。每层只记录一次，因此答案与树的深度一一对应，不会漏层也不会重复。
//
// 6. 边界与易错点
//   不要在 for 条件里动态使用 `pending.size()` 作为当前层长度，因为处理中会持续加入下一层孩子；必须先冻结 width。若改成右孩子先入队，则“最后一个”不再代表最右节点，需要相应改成记录第一个。空树要返回空数组。
//
// 7. 举一反三
//   它是 LC-102 的最小改造：BFS 框架、frontier、width 不变量全部不变，只改变“每层如何聚合”。同样可以得到每层最大值、平均值、最左视图、锯齿层序等；新题先问的应是“遍历结构是否已学过，真正变化的只是层内统计规则吗？”
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个层序节点值（-1 表示空节点）。
//   输出：右视图节点值，以空格分隔。
// test.in 的预期输出：1 3 4
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
    vector<int> rightSideView(TreeNode* root) {
        vector<int> answer;
        if (!root) return answer;
        queue<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            const int width = static_cast<int>(pending.size());
            for (int i = 0; i < width; ++i) {
                TreeNode* node = pending.front();
                pending.pop();
                if (i == width - 1) answer.push_back(node->val);
                if (node->left) pending.push(node->left);
                if (node->right) pending.push(node->right);
            }
        }
        return answer;
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
    auto ans = sol.rightSideView(buildTreeLO(a));
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
