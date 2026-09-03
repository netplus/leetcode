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
// 约束与要求：
//   - 树中节点数目在范围 [0, 2000] 内
//   - -1000 <= Node.val <= 1000
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜队列 BFS：按当前层大小分组
// - 核心要点：
//   1. 思路起点：队列按距离根的层次顺序保存节点；进入一层时记录 queue.size()，只处理这批节点，新加入的孩子留给下一层。
//   2. 执行逻辑：1. 根非空时入队；2. 每轮读取当前队列大小并创建层结果；3. 弹出固定数量节点，同时压入左右孩子。
//   3. 为什么这样做：一轮开始时队列恰含当前层全部节点；固定次数处理后，这些节点全部出队且其非空孩子恰构成下一层。
// - 边界与易错点：不能在本层循环中使用不断变化的 queue.size() 作上界；空树返回空二维数组。
// - 举一反三：多源 BFS、最短无权路径和逐层统计都依赖队列中“同一距离层”的边界。
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

