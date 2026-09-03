// ============================================================================
// LC-542：01 矩阵
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 16 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个由 0 和 1 组成的矩阵 mat，请输出一个大小相同的矩阵，其中每一个格子是 mat 中对应位置元素到最近的 0 的距离。
// 两个相邻元素间的距离为 1。
//
// 约束与要求：
//   - m == mat.length
//   - n == mat[i].length
//   - 1 <= m, n <= 10^4
//   - 1 <= m * n <= 10^4
//   - mat[i][j] 为 0 或 1。
//   - mat 中至少有一个 0
//
// 复杂度目标：O(m*n) 时间（多源 BFS）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜多源 BFS：从所有 0 反向扩散距离
// - 核心要点：
//   1. 思路起点：若从每个 1 单独寻找最近 0 会重复搜索；把所有 0 同时作为源，首次到达每个 1 时就是其最短曼哈顿距离。
//   2. 执行逻辑：1. 0 的距离设为 0 并全部入队，1 先设为 -1；2. 弹出格子并访问四邻；3. 只给尚未访问格赋 current+1 并入队。
//   3. 为什么这样做：BFS 队列按距离非降序处理，多源可视为连接到虚拟超级源；因此未访问邻居的首次赋值必为到任一 0 的最短距离。
// - 边界与易错点：需要独立未访问标记或复用结果中的 -1；所有 0 必须同时入队；四向距离不是对角线距离。
// - 举一反三：“求每点到最近某类节点”通常反向从目标集合做多源 BFS，比逐点搜索高效。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：n 个以空格分隔的整数。
//   输出：距离矩阵，每行输出矩阵的一行，元素以空格分隔。
// test.in 的预期输出：0 0 0 | 0 1 0 | 1 2 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        const int rows = static_cast<int>(mat.size());
        const int cols = static_cast<int>(mat[0].size());
        vector<vector<int>> distance(rows, vector<int>(cols, -1));
        queue<pair<int, int>> pending;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (mat[r][c] == 0) {
                    distance[r][c] = 0;
                    pending.push({r, c});
                }
            }
        }

        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        while (!pending.empty()) {
            auto [r, c] = pending.front();
            pending.pop();
            for (auto& d : directions) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && distance[nr][nc] == -1) {
                    distance[nr][nc] = distance[r][c] + 1;
                    pending.push({nr, nc});
                }
            }
        }
        return distance;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<int>> g(m, vector<int>(n));
    for (int i = 0; i < m; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    auto res = sol.updateMatrix(g);
    for (auto& r : res) for (size_t j = 0; j < r.size(); ++j) cout << r[j] << " \n"[j + 1 == r.size()];
    return 0;
}

