// ============================================================================
// LC-695：岛屿的最大面积
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 15 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个大小为 m x n 的二进制矩阵 grid。
// 岛屿是由一些相邻的 1 (代表土地) 构成的组合，这里的「相邻」要求两个 1 必须在水平或者竖直的四个方向上相邻。
// 你可以假设 grid 的四个边缘都被 0 （代表水）包围着。
// 岛屿的面积是岛上值为 1 的单元格的数目。
// 计算并返回 grid 中最大的岛屿面积。
// 如果没有岛屿，则返回面积为 0。
//
// 约束与要求：
//   - m == grid.length
//   - n == grid[i].length
//   - 1 <= m, n <= 50
//   - grid[i][j] 为 0 或 1
//
// 复杂度目标：O(m*n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜网格 BFS：累计连通分量面积
// - 核心要点：
//   1. 思路起点：与岛屿计数相同，但每次洪泛时累计弹出的陆地格数，并用它更新最大面积。
//   2. 执行逻辑：1. 扫描未访问的 1；2. BFS 入队染色并令 area 逐格加一；3. 一个分量结束后更新 best。
//   3. 为什么这样做：BFS 访问集合恰是起点所在四连通分量，每格贡献一次，所以 area 是该岛面积；对所有分量取最大即答案。
// - 边界与易错点：全水域答案为 0；入队即改为 0；若调用方要求保留 grid，应改用独立 visited 数组。
// - 举一反三：把分量内的累加器替换为周长、颜色频次或边界框，就能解决一族网格聚合问题。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：n 个以空格分隔的整数。
//   输出：岛屿的最大面积。
// test.in 的预期输出：6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int best = 0;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != 1) continue;
                int area = 0;
                queue<pair<int, int>> pending;
                pending.push({r, c});
                grid[r][c] = 0;
                while (!pending.empty()) {
                    auto [x, y] = pending.front();
                    pending.pop();
                    ++area;
                    for (auto& d : directions) {
                        int nx = x + d[0], ny = y + d[1];
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == 1) {
                            grid[nx][ny] = 0;
                            pending.push({nx, ny});
                        }
                    }
                }
                best = max(best, area);
            }
        }
        return best;
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
    cout << sol.maxAreaOfIsland(g) << "\n";
    return 0;
}

