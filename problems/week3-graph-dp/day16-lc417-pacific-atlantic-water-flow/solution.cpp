// ============================================================================
// LC-417：太平洋大西洋水流问题
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 16 天
// ----------------------------------------------------------------------------
// 题目描述：
// 有一个 m × n 的矩形岛屿，与太平洋和大西洋相邻。
// “太平洋” 处于大陆的左边界和上边界，而 “大西洋” 处于大陆的右边界和下边界。
// 这个岛被分割成一个由若干方形单元格组成的网格。
// 给定一个 m x n 的整数矩阵 heights，heights[r][c] 表示坐标 (r, c) 上单元格高于海平面的高度。
// 岛上雨水较多，如果相邻单元格的高度小于或等于当前单元格的高度，雨水可以直接向北、南、东、西流向相邻单元格。
// 水可以从海洋附近的任何单元格流入海洋。
// 返回网格坐标 result 的 2D 列表，其中 result[i] = [r_i, c_i] 表示雨水从单元格 (ri, ci) 流动既可流向太平洋也可流向大西洋。
//
// 约束与要求：
//   - m == heights.length
//   - n == heights[r].length
//   - 1 <= m, n <= 200
//   - 0 <= heights[r][c] <= 10^5
//
// 复杂度目标：O(m*n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜反向洪泛：从海岸寻找可逆到达点
// - 核心要点：
//   1. 思路起点：正向从每格向不高于自己的邻格流水代价高；反向从海岸出发，只走到高度不低的邻格，得到能流向该海洋的所有格。
//   2. 执行逻辑：1. 分别把太平洋边界和大西洋边界作为多源起点；2. 反向 BFS 只允许 nextHeight>=currentHeight；3. 取两份可达标记的交集。
//   3. 为什么这样做：反向边恰是原流水边的逆边，所以从海岸反向可达当且仅当该格正向能流到海岸；同时在两集合中即能到两洋。
// - 边界与易错点：四条边的角落可能重复入队，需先检查 visited；两个海洋必须使用独立标记；相等高度允许流动。
// - 举一反三：当终点集合很小而起点很多时，反转图从终点做多源搜索常能消除重复工作。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   接下来 m 行：n 个以空格分隔的整数。
//   输出：所有坐标，排序后每行输出一组 "r c"。
// test.in 的预期输出：0 4 | 1 3 | 1 4 | 2 2 | 3 0 | 3 1 | 4 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        const int rows = static_cast<int>(heights.size());
        const int cols = static_cast<int>(heights[0].size());
        vector<vector<char>> pacific(rows, vector<char>(cols, false));
        vector<vector<char>> atlantic(rows, vector<char>(cols, false));

        auto flood = [&](vector<vector<char>>& seen, vector<pair<int, int>> starts) {
            queue<pair<int, int>> pending;
            for (auto [r, c] : starts) {
                if (!seen[r][c]) {
                    seen[r][c] = true;
                    pending.push({r, c});
                }
            }
            const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
            while (!pending.empty()) {
                auto [r, c] = pending.front();
                pending.pop();
                for (auto& d : directions) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !seen[nr][nc]
                        && heights[nr][nc] >= heights[r][c]) {
                        seen[nr][nc] = true;
                        pending.push({nr, nc});
                    }
                }
            }
        };

        vector<pair<int, int>> pStarts, aStarts;
        for (int r = 0; r < rows; ++r) {
            pStarts.push_back({r, 0});
            aStarts.push_back({r, cols - 1});
        }
        for (int c = 0; c < cols; ++c) {
            pStarts.push_back({0, c});
            aStarts.push_back({rows - 1, c});
        }
        flood(pacific, move(pStarts));
        flood(atlantic, move(aStarts));

        vector<vector<int>> answer;
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (pacific[r][c] && atlantic[r][c]) answer.push_back({r, c});
        return answer;
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
    auto res = sol.pacificAtlantic(g);
    sort(res.begin(), res.end());
    for (auto& c : res) cout << c[0] << " " << c[1] << "\n";
    return 0;
}

