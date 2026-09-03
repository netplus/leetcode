// ============================================================================
// LC-994：腐烂的橘子
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 16 天
// ----------------------------------------------------------------------------
// 题目描述：
// 在给定的 m x n 网格 grid 中，每个单元格可以有以下三个值之一：值 0 代表空单元格；值 1 代表新鲜橘子；值 2 代表腐烂的橘子。
// 每分钟，腐烂的橘子周围 4 个方向上相邻的新鲜橘子都会腐烂。
// 返回直到单元格中没有新鲜橘子为止所必须经过的最小分钟数。
// 如果不可能，返回 -1。
//
// 约束与要求：
//   - m == grid.length
//   - n == grid[i].length
//   - 1 <= m, n <= 10
//   - grid[i][j] 仅为 0、1 或 2
//
// 复杂度目标：O(m*n) 时间（多源 BFS）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜多源 BFS：按分钟同步扩散
// - 核心要点：
//   1. 思路起点：所有初始腐烂橘子在时间 0 同时作为 BFS 源；一层扩展对应一分钟，新腐烂橘子进入下一层。
//   2. 执行逻辑：1. 统计新鲜橘子并把所有腐烂橘子入队；2. 每轮固定当前队列大小并令 minutes++；3. 感染四邻新鲜橘子、减少 fresh；最后判断是否清零。
//   3. 为什么这样做：多源 BFS 首次到达某格的层数就是它到最近初始腐烂橘子的最短距离；逐层并行扩散与题目时间过程等价。
// - 边界与易错点：没有新鲜橘子应返回 0；隔离的新鲜橘子最终返回 -1；分钟只在 fresh>0 且确有当前层时增加。
// - 举一反三：火灾传播、最近设施距离和多个起点的无权最短路都可把所有源同时入队。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：n 个以空格分隔的整数。
//   输出：所需的最少分钟数；无法完成时输出 -1。
// test.in 的预期输出：4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        queue<pair<int, int>> rotten;
        int fresh = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] == 2) rotten.push({r, c});
                else if (grid[r][c] == 1) ++fresh;
            }
        }

        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int minutes = 0;
        while (!rotten.empty() && fresh > 0) {
            int layer = static_cast<int>(rotten.size());
            ++minutes;
            while (layer--) {
                auto [r, c] = rotten.front();
                rotten.pop();
                for (auto& d : directions) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == 1) {
                        grid[nr][nc] = 2;
                        --fresh;
                        rotten.push({nr, nc});
                    }
                }
            }
        }
        return fresh == 0 ? minutes : -1;
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
    cout << sol.orangesRotting(g) << "\n";
    return 0;
}

