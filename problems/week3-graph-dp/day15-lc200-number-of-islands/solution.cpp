// ============================================================================
// LC-200：岛屿数量
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 15 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个由 '1' （陆地）和 '0' （水）组成的的二维网格，请你计算网格中岛屿的数量。
// 岛屿总是被水包围，并且每座岛屿只能由水平方向和/或竖直方向上相邻的陆地连接形成。
// 此外，你可以假设该网格的四条边均被水包围。
//
// 约束与要求：
//   - m == grid.length
//   - n == grid[i].length
//   - 1 <= m, n <= 300
//   - grid[i][j] 的值为 '0' 或 '1'
//
// 复杂度目标：O(m*n) 时间，O(m*n) 空间（DFS/BFS）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜网格连通分量：BFS 染色
// - 核心要点：
//   1. 思路起点：每个岛屿是上下左右相邻的 '1' 构成的连通分量；发现未访问陆地时答案加一，并一次淹没整个分量。
//   2. 执行逻辑：1. 扫描所有格子；2. 遇到 '1' 就入队并立即标为 '0'；3. BFS 扩展四邻域直到该岛全部处理。
//   3. 为什么这样做：每次启动 BFS 的格子不属于此前任何分量；BFS 恰访问与它连通的全部陆地并标记，故每个岛只计数一次且无遗漏。
// - 边界与易错点：必须在入队时标记，避免同一格重复入队；只允许四方向而非对角线；空网格应直接返回 0。
// - 举一反三：网格岛屿、区域填充、连通块面积与封闭区域都共享“扫描起点 + 洪泛标记”框架。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：每行是由 n 个字符（'0'/'1'）组成的字符串。
//   输出：岛屿数量。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int islands = 0;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != '1') continue;
                ++islands;
                queue<pair<int, int>> pending;
                pending.push({r, c});
                grid[r][c] = '0';  // 入队即标记，杜绝重复入队
                while (!pending.empty()) {
                    auto [x, y] = pending.front();
                    pending.pop();
                    for (auto& d : directions) {
                        int nx = x + d[0], ny = y + d[1];
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == '1') {
                            grid[nx][ny] = '0';
                            pending.push({nx, ny});
                        }
                    }
                }
            }
        }
        return islands;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<vector<char>> g(m, vector<char>(n));
    for (int i = 0; i < m; ++i) {
        string row; cin >> row;
        for (int j = 0; j < n; ++j) g[i][j] = row[j];
    }
    cout << sol.numIslands(g) << "\n";
    return 0;
}

