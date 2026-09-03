// ============================================================================
// LC-547：省份数量
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 17 天
// ----------------------------------------------------------------------------
// 题目描述：
// 有 n 个城市，其中一些彼此相连，另一些没有相连。
// 如果城市 a 与城市 b 直接相连，且城市 b 与城市 c 直接相连，那么城市 a 与城市 c 间接相连。
// 省份是一组直接或间接相连的城市，组内不含其他没有相连的城市。
// 给你一个 n x n 的矩阵 isConnected，其中 isConnected[i][j] = 1 表示第 i 个城市和第 j 个城市直接相连，而 isConnected[i][j] = 0
// 表示二者不直接相连。
// 返回矩阵中省份的数量。
//
// 约束与要求：
//   - 1 <= n <= 200
//   - n == isConnected.length
//   - n == isConnected[i].length
//   - isConnected[i][j] 为 1 或 0
//   - isConnected[i][i] == 1
//   - isConnected[i][j] == isConnected[j][i]
//
// 复杂度目标：O(n^2) 时间，O(n) 空间（并查集）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜并查集：动态合并连通关系
// - 核心要点：
//   1. 思路起点：每个城市初始自成集合；矩阵中的连接把两个集合合并，最终根节点数量就是省份数。
//   2. 执行逻辑：1. 初始化 parent、size 和 components=n；2. 只扫描矩阵上三角的连接；3. find 路径压缩，union 按大小合并并令 components--。
//   3. 为什么这样做：并查集等价类始终与已处理边的连通分量一致；合并不同根恰使分量数减一，同根边不改变分量。
// - 边界与易错点：矩阵对称且对角线为 1，只扫 j>i 可避免重复；components 只在真正合并时减少。
// - 举一反三：并查集适合离线连通性、冗余边、账户合并和 Kruskal 最小生成树。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   接下来 n 行：n 个以空格分隔的整数。
//   输出：省份数量。
// test.in 的预期输出：2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    struct DSU {
        vector<int> parent, size;
        int components;
        explicit DSU(int n) : parent(n), size(n, 1), components(n) {
            iota(parent.begin(), parent.end(), 0);
        }
        int find(int x) {
            if (parent[x] != x) parent[x] = find(parent[x]);
            return parent[x];
        }
        void unite(int a, int b) {
            a = find(a); b = find(b);
            if (a == b) return;
            if (size[a] < size[b]) swap(a, b);
            parent[b] = a;
            size[a] += size[b];
            --components;
        }
    };

public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        const int n = static_cast<int>(isConnected.size());
        DSU dsu(n);
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (isConnected[i][j]) dsu.unite(i, j);
        return dsu.components;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<vector<int>> g(n, vector<int>(n));
    for (int i = 0; i < n; ++i) for (int j = 0; j < n; ++j) cin >> g[i][j];
    cout << sol.findCircleNum(g) << "\n";
    return 0;
}

