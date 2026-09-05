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
// 解法精讲｜反向多源可达性：把“每个格子能否到终点”改成“终点能反向到哪些格子”
//
// 0. 优化是怎么来的
//   最直接的正确做法，是对每一个格子 (r,c) 分别执行搜索：沿着 nextHeight<=currentHeight 的真实水流方向，看能否碰到太平洋边界、又能否碰到大西洋边界。单次搜索最坏 O(mn)，一共有 mn 个起点，因此最坏可能达到 O((mn)^2)，而且相邻起点会一遍遍探索相同区域。
//
//   问题问的是“很多起点能否到少数终点集合”。把边方向反过来后，关系完全等价：
//   A 正向能到 Ocean
//   <=> Ocean 在反向图中能到 A。
//
//   于是无需从 mn 个起点重复问同一个问题。把太平洋全部边界格同时作为源做一次反向洪泛，得到 pacific；再对大西洋做一次，得到 atlantic。每个格子在每次洪泛中最多访问一次。
//
//   优化类型：每个起点重复可达性搜索 -> 反转边 + 从终点集合做多源搜索；复杂度压到 O(mn)。
//
// 1. 图像直觉
//   正向水流规则：
//
//   高 5  --->  低 3
//   只允许往不更高的位置流。
//
//   把问题反过来看：
//
//   Ocean ---> 岸边 ---> 内陆
//                3   ---> 5
//
//   反向搜索只能“爬坡”或走平地：
//   nextHeight >= currentHeight
//
//   太平洋源集合：上边界 + 左边界
//   P P P P
//   P . . .
//   P . . .
//
//   大西洋源集合：下边界 + 右边界
//   . . . A
//   . . . A
//   A A A A
//
//   分别反向洪泛后：
//   Pseen ∩ Aseen
//   就是原方向下同时能流向两边海洋的格子。
//
// 2. 一句话核心
//   不要从每个格子向海洋重复找路；把水流边反过来，从两个海洋边界各做一次多源洪泛，最后取两份可达集合的交集。
//
// 3. 公式 / 不变量
//   原方向允许：
//   height[next] <= height[current]
//
//   反向边允许：
//   height[next] >= height[current]
//
//   定义：
//   pacific[r][c] = 太平洋边界在反向图中能否到达 (r,c)
//   atlantic[r][c] = 大西洋边界在反向图中能否到达 (r,c)
//
//   由反向可达等价：
//   (r,c) 正向能到太平洋 <=> pacific[r][c] = true
//   (r,c) 正向能到大西洋 <=> atlantic[r][c] = true
//
//   最终条件：
//   pacific[r][c] && atlantic[r][c]。
//
// 4. 执行步骤
//   1. 准备两张独立 seen 表 pacific/atlantic；同一个格子对两个海洋的可达性是两份不同状态
//   2. 把上边界与左边界收集为太平洋多源起点，把下边界与右边界收集为大西洋多源起点
//   3. flood 初始化时对起点先检查 seen，避免四个角因为属于两条边而在同一次洪泛中重复入队
//   4. BFS 每次只向未访问且 heights[nr][nc]>=heights[r][c] 的邻格前进，这正是原水流边的逆边
//   5. 两次 flood 完成后扫描整个矩阵，只收集 pacific 与 atlantic 同时为 true 的坐标
//   记忆：起点太多、终点很少时先问能否反边：从终点一起往回找。
//
// 5. 为什么不会漏 / 不会重
//   若某格 x 正向能沿合法水流边到达海洋，则把该路径完全倒过来，每一步都满足反向的“不下降”条件，所以海岸源在反向搜索中一定能到 x。反之，任何海岸到 x 的反向路径倒过来后都成为 x 到海岸的合法水流路径。因此两份 seen 分别精确描述到两个海洋的正向可达集合，取交集既不会漏也不会多。
//
// 6. 边界与易错点
//   相等高度在原题允许流动，所以反向条件必须是 >=，不能写 >。两个海洋必须有独立 seen。角落会同时出现在同一海洋的两条边源集合中，入队前检查 seen 即可去重。当前实现使用 structured binding、lambda、move，均属于 C++17 或更早；没有依赖 C++20 API。
//
// 7. 举一反三
//   它和 LC-542 的共同思想是“很多位置都在问离/能否到某类目标”，于是从目标集合反向统一搜索。更一般地，在有向图中如果问题是大量节点到固定终点集合的可达性，可以考虑反图；在无权图中若目标集合是多个点，再叠加多源 BFS。
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

        // 同一个格子“能否到太平洋”和“能否到大西洋”是两份独立可达状态，不能共用 visited。
        vector<vector<char>> pacific(rows, vector<char>(cols, false));
        vector<vector<char>> atlantic(rows, vector<char>(cols, false));

        // flood 在“反向水流图”里做一次多源 BFS：seen 表示当前这一个海洋能反向到达哪些内陆格。
        // starts 按值传入，是一份本次洪泛专用的源列表；调用处用 move 只是在避免额外复制。
        auto flood = [&](vector<vector<char>>& seen, vector<pair<int, int>> starts) {
            queue<pair<int, int>> pending;

            // 同一海洋的两条边会在角落产生重复源；入队前先检查 seen，确保每个源格只进入队列一次。
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

                    // 原水流允许 high -> low/equal；把边反过来以后，海洋向内陆只能走到 high/equal：
                    // heights[next] >= heights[current]。首次发现时立即标记，避免重复入队。
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !seen[nr][nc]
                        && heights[nr][nc] >= heights[r][c]) {
                        seen[nr][nc] = true;
                        pending.push({nr, nc});
                    }
                }
            }
        };

        vector<pair<int, int>> pStarts, aStarts;

        // 太平洋接触左边界，大西洋接触右边界。
        for (int r = 0; r < rows; ++r) {
            pStarts.push_back({r, 0});
            aStarts.push_back({r, cols - 1});
        }

        // 太平洋接触上边界，大西洋接触下边界；四个角的重复会由 flood 的 seen 检查消掉。
        for (int c = 0; c < cols; ++c) {
            pStarts.push_back({0, c});
            aStarts.push_back({rows - 1, c});
        }

        // move 只是把临时源列表的存储交给 lambda 参数，减少一次 vector 拷贝；不改变多源 BFS 语义。
        flood(pacific, move(pStarts));
        flood(atlantic, move(aStarts));

        vector<vector<int>> answer;
        // 一个格子原方向能同时流向两海洋，当且仅当它属于两次反向可达集合的交集。
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
