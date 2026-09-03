// ============================================================================
// LC-685：冗余连接 II
// 难度：困难
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 17 天
// ----------------------------------------------------------------------------
// 题目描述：
// 在本问题中，有根树指满足以下条件的有向图。
// 该树只有一个根节点，所有其他节点都是该根节点的后继。
// 该树除了根节点之外的每一个节点都有且只有一个父节点，而根节点没有父节点。
// 输入一个有向图，该图由一个有着 n 个节点（节点值不重复，从 1 到 n）的树及一条附加的有向边构成。
// 附加的边包含在 1 到 n 中的两个不同顶点间，这条附加的边不属于树中已存在的边。
// 结果图是一个以边组成的二维数组 edges。
// 每个元素是一对 [u_i, v_i]，用以表示有向图中连接顶点 u_i 和顶点 v_i 的边，其中 u_i 是 v_i 的一个父节点。
// 返回一条能删除的边，使得剩下的图是有 n 个节点的有根树。
// 若有多个答案，返回最后出现在给定二维数组的答案。
//
// 约束与要求：
//   - n == edges.length
//   - 3 <= n <= 1000
//   - edges[i].length == 2
//   - 1 <= u_i, v_i <= n
//
// 复杂度目标：O(n) 并查集。
//
// ----------------------------------------------------------------------------
// 解法精讲｜有向树冗余边：二父节点与有向环分类
// - 核心要点：
//   1. 思路起点：非法结构只有两类症状：某节点入度为 2、或存在环；先记录造成二父的两条候选边，再跳过较晚边做并查集判环。
//   2. 执行逻辑：1. 扫描父节点记录，得到 earlier/later 候选；2. 并查集重放所有边但跳过 later；3. 若仍成环返回 earlier；无环返回 later；无二父时返回成环边。
//   3. 为什么这样做：跳过 later 后若无环，其删除即可恢复每点单父的树；若仍有环，环必包含 earlier，删除它才能同时消除环和二父；无二父时唯一问题就是环。
// - 边界与易错点：这是有向问题，不能直接套无向冗余边；候选需保留输入顺序；并查集只用于检测忽略方向后的环。
// - 举一反三：复杂结构题常先按违反的局部约束分类，再用一个简化判定器验证候选，而非一次性硬写所有分支。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m (边数，等于 n)。
//   接下来 m 行：u v (u 是 v 的父节点)。
//   输出：冗余边 "u v"。
// test.in 的预期输出：2 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        const int n = static_cast<int>(edges.size());
        vector<int> directParent(n + 1, 0);
        int earlier = -1, later = -1;
        for (int i = 0; i < n; ++i) {
            int from = edges[i][0], to = edges[i][1];
            if (directParent[to] == 0) {
                directParent[to] = from;
            } else {
                for (int j = 0; j < i; ++j) {
                    if (edges[j][1] == to) { earlier = j; break; }
                }
                later = i;
                break;
            }
        }

        vector<int> parent(n + 1);
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        for (int i = 0; i < n; ++i) {
            if (i == later) continue;
            int a = find(edges[i][0]), b = find(edges[i][1]);
            if (a == b) {
                return earlier == -1 ? edges[i] : edges[earlier];
            }
            parent[b] = a;
        }
        return edges[later];
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> e(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> e[i][0] >> e[i][1];
    auto ans = sol.findRedundantDirectedConnection(e);
    cout << ans[0] << " " << ans[1] << "\n";
    return 0;
}

