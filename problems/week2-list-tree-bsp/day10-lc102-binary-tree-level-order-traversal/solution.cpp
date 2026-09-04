// ============================================================================
// LC-102：二叉树的层序遍历
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 10 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你二叉树的根节点 root，返回其节点值的层序遍历。
// （即逐层地，从左到右访问所有节点）。
//
// 题目示例：
// 示例 1：
//   Input: root = [3,9,20,null,null,15,7]
//   Output: [[3],[9,20],[15,7]]
//
// 示例 2：
//   Input: root = [1]
//   Output: [[1]]
//
// 示例 3：
//   Input: root = []
//   Output: []
//
// 约束与要求：
//   - 树中节点数目在范围 [0, 2000] 内
//   - -1000 <= Node.val <= 1000
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜层序遍历：BFS 队列 + 冻结当前层宽度
//
// 1. 图像直觉
//   把树按距离根的层次画开：
//
//   level 0:        3
//                  |
//   level 1:     9    20
//                     / \
//   level 2:        15   7
//
//   队列始终从左到右保存“接下来要处理的节点”。
//   进入某一轮时：
//   queue = [9, 20]
//   width = 2   <- 先冻结
//
//   处理 9、20 的过程中会把 15、7 放到队尾：
//   queue = [15, 7]
//
//   关键是本轮只循环原来的 width=2 次；新孩子属于下一层，不能混进本层。
//
// 2. 一句话核心
//   每轮开始先冻结 queue.size()，这个数字就是当前层节点数；只弹这批节点，新入队的孩子天然组成下一层。
//
// 3. 公式 / 不变量
//   一轮开始的不变量：
//   queue = 当前层全部节点（从左到右）
//   width = queue.size()
//
//   循环恰好 width 次后：
//   - 当前层全部出队；
//   - 每个当前层节点按 left、right 顺序加入孩子；
//   - queue 恰好变成下一层全部节点。
//
//   这就是 BFS 的“距离分层”性质。
//
// 4. 执行步骤
//   1. 空树返回空结果；非空时只把 root 放入队列
//   2. 每轮开始读取 width=pending.size()，创建这一层的结果数组
//   3. 恰好循环 width 次：弹出节点、记录值，并按左后右顺序把非空孩子加入队尾
//   4. 这一批处理完成后保存 level；此时队列只剩下一层，继续下一轮
//   记忆：先拍一张当前队列长度的快照，再处理这一批；新来的孩子下一轮再说。
//
// 5. 为什么不会漏 / 不会重
//   根显然构成第 0 层。假设某轮开始队列恰好是第 d 层，循环只消费这固定的 width 个节点，因此不会误吃刚加入的下一层节点；每个第 d+1 层节点都有唯一父节点，并在处理父节点时恰好入队一次，所以循环结束后队列恰好且仅包含第 d+1 层。归纳即可覆盖所有层且无重复。
//
// 6. 边界与易错点
//   不能写成 for (i=0; i<pending.size(); ++i)，因为 size 在循环中会随着孩子入队变化，层边界会被破坏。左右孩子入队顺序决定同层输出顺序。空树不能把 nullptr 入队后再统一处理，否则会增加无意义分支。
//
// 7. 举一反三
//   这一题建立的 frontier 模型会直接迁移到 Week 3 图 BFS：腐烂橘子、01 矩阵、多源 BFS 都是在一层层推进“距离”。树只是一个天然无环、无需 visited 的特殊图。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的层序节点值（-1 表示空节点）。
//   输出：层序遍历结果，每层一行且以空格分隔。
// test.in 的预期输出：3 | 9 20 | 15 7
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
    vector<vector<int>> levelOrder(TreeNode* root) {
        vector<vector<int>> levels;
        if (!root) return levels;
        queue<TreeNode*> pending;
        pending.push(root);
        while (!pending.empty()) {
            const int width = static_cast<int>(pending.size());
            vector<int> level;
            level.reserve(width);
            for (int i = 0; i < width; ++i) {
                TreeNode* node = pending.front();
                pending.pop();
                level.push_back(node->val);
                if (node->left) pending.push(node->left);
                if (node->right) pending.push(node->right);
            }
            levels.push_back(move(level));
        }
        return levels;
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
    auto res = sol.levelOrder(buildTreeLO(a));
    for (auto& lvl : res) {
        for (size_t i = 0; i < lvl.size(); ++i) cout << lvl[i] << " \n"[i + 1 == lvl.size()];
    }
    return 0;
}
