// ============================================================================
// LC-200：岛屿数量｜补充实现：递归 DFS
//
// 主文件 solution.cpp 使用 BFS + 显式 queue 迭代遍历。
// 本文件保留同一个“外层找新岛，内层吃完整岛”的模型，只把局部洪泛改成递归 DFS：
//
//   BFS：显式 queue 保存“以后还要处理哪些格子”
//   DFS：函数调用栈隐式保存“当前递归返回后还要继续处理什么”
//
// 两种方法都会把第一次访问到的陆地立即从 '1' 改成 '0'，因此同一格只处理一次。
// 时间复杂度都是 O(m*n)。递归 DFS 的调用栈最坏可达到 O(m*n)；对于很大的蛇形
// 连通块可能触发栈深限制，所以仓库仍保留迭代 BFS 作为默认实现。
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

class Solution {
private:
    void flood(vector<vector<char>>& grid, int r, int c) {
        // 递归的停止条件有两类：
        // 1) 坐标已经走出网格；
        // 2) 当前不是尚未访问的陆地（是水，或已经被前面的 DFS 染成 '0'）。
        // 必须先判断边界，再读取 grid[r][c]，避免非法下标访问。
        if (r < 0 || r >= static_cast<int>(grid.size()) ||
            c < 0 || c >= static_cast<int>(grid[0].size()) ||
            grid[r][c] != '1') {
            return;
        }

        // 一进入合法陆地就立即标记，而不是等四个方向递归完成后再标记。
        // 这样相邻格沿反方向走回来时会看到 '0' 并立即返回，不会 A -> B -> A 无限递归。
        grid[r][c] = '0';

        // 递归调用的含义不是“数岛”，而是：
        // 从当前格继续把同一座岛中四方向可达的所有陆地全部处理掉。
        flood(grid, r - 1, c);  // 上
        flood(grid, r + 1, c);  // 下
        flood(grid, r, c - 1);  // 左
        flood(grid, r, c + 1);  // 右
    }

public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;

        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        int islands = 0;

        // 外层扫描只负责寻找“尚未属于此前任何已处理连通分量”的起点。
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != '1') continue;

                // 扫描到这里仍然是 '1'，说明此前任何一次 DFS 都没有到达它，
                // 因而它属于一座新的岛。计数一次，再用 flood() 一次吃掉整座岛。
                ++islands;
                flood(grid, r, c);
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
    int m, n;
    if (!(cin >> m >> n)) return 0;

    vector<vector<char>> grid(m, vector<char>(n));
    for (int r = 0; r < m; ++r) {
        string row;
        cin >> row;
        for (int c = 0; c < n; ++c) {
            grid[r][c] = row[c];
        }
    }

    cout << sol.numIslands(grid) << '\n';
    return 0;
}
