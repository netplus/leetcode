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
// 解法精讲｜并查集母模型：把连通分量压缩成可持续维护的代表元
//
// 0. 优化是怎么来的
//   最自然的正确解法，是直接把 isConnected 当成无向图：外层枚举城市，遇到未访问城市就 DFS/BFS，把所有可达城市标记 visited，并让答案加一。因为输入本身就是 n×n 矩阵，读取邻接关系已经需要 O(n^2)，这个方案清晰而且完全合格。
//
//   为什么这一题还值得学习并查集？因为后面的 LC-684/685 不再只是“最后数一次连通块”，而是边一条条到来时反复问：这两个点现在是否已经连通？如果每次都重新 DFS，会重复遍历旧图。
//
//   所以这里先换一种表达方式：每个集合只保存一个代表根 parent；看到连接 i-j 时，不重新搜索整个分量，而是把两个根合并。`find(x)` 回答“x 当前属于哪个分量”，`unite(a,b)` 回答“把两个分量合成一个”。
//
//   对 LC-547 单题而言，这是与 DFS/BFS 同量级的替代表达；真正可复用的优化价值，是把未来大量连通性查询压缩成近常数时间的 find/union。
//
// 1. 图像直觉
//   初始时 5 个城市各自成组：
//
//   0   1   2   3   4
//   |   |   |   |   |
//   0   1   2   3   4      components = 5
//
//   读到 0--1：
//   0---1   2   3   4      components = 4
//     根0
//
//   再读到 1--2：
//   0---1---2   3   4      components = 3
//     同一个根
//
//   如果再看到 0--2，它们的 find 已经相同：
//   find(0) == find(2)
//   => 这条边没有合并两个不同省份
//   => components 不变。
//
//   并查集不保存“从 0 到 2 的具体路径”；它只保存更粗但正好够用的信息：
//   “0、1、2 现在属于同一个集合”。
//
// 2. 一句话核心
//   每个连通分量只保留一个代表根；连接两个城市时只合并它们的根，真正合并成功一次，连通分量数量就减一。
//
// 3. 公式 / 不变量
//   状态：
//   parent[x] = x 当前沿父指针所属集合的代表关系
//   size[root] = 该根对应集合大小
//   components = 当前连通分量数量
//
//   find(x)：
//   若 parent[x] != x，递归找到根，并执行路径压缩：
//   parent[x] = find(parent[x])
//
//   unite(a,b)：
//   ra = find(a)
//   rb = find(b)
//
//   若 ra == rb：
//   已经属于同一分量，不做任何事。
//
//   若 ra != rb：
//   按 size 把小树挂到大树下；
//   components--。
//
//   关键不变量：处理完当前已经扫描的所有连接后，
//   find(a) == find(b)
//   当且仅当 a、b 已被这些连接放进同一连通分量。
//
// 4. 执行步骤
//   1. 初始化 parent[i]=i、size[i]=1、components=n；每个城市先独立成为一个省份
//   2. 利用矩阵对称性只扫描 j>i 的上三角；对角线和下三角都不提供新的连接信息
//   3. 遇到 isConnected[i][j]==1 时调用 unite(i,j)，先分别 find 两端代表根
//   4. 若两根相同说明此前已经间接连通，跳过；若不同则按 size 合并并令 components 减一
//   5. 全部边处理完后直接返回 components，它就是最终连通分量数量
//   记忆：DFS 是走完整个分量；DSU 是给整个分量留一个代表。不同根合并一次，分量数减一。
//
// 5. 为什么不会漏 / 不会重
//   初始时没有处理任何城市间连接，每个节点独立成分量，与并查集状态一致。每处理一条边 (a,b)：若两端已经同根，这条边只是在同一连通分量内部增加一条连接，不会改变分量划分；若不同根，这条边恰好把两个原本独立的连通分量连接成一个，因此合并并让 components 减一正好保持真实图与 DSU 分组一致。归纳到所有边处理结束，DSU 集合与图的省份一一对应。
//
// 6. 边界与易错点
//   不要因为本题用了 DSU 就声称它比 DFS/BFS 在 n×n 邻接矩阵上渐进更快；扫描矩阵本身就是 O(n^2)。`components` 只能在两个不同根真正合并时减一。矩阵对称且 `isConnected[i][i]==1`，只扫上三角可避免重复 union。当前实现同时使用路径压缩和按大小合并，二者让连续 find/union 的摊还代价接近常数。
//
// 7. 举一反三
//   把它与 LC-200 对照：DFS/BFS 保存的是“这次搜索访问过谁”，DSU 保存的是“长期来看谁和谁属于同一组”。下一题 LC-684 会让边按顺序到来，并在每条边加入前问两端是否已经同组——这正是本题提前建立代表元模型的原因。
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
