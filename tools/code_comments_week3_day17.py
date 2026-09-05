"""Reviewed C++ key-code comments for Week 3 / Day 17.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK3_DAY17 = {
    547: r'''// ---------- Solution ----------
class Solution {
    struct DSU {
        // parent[x] 沿父指针最终指向当前连通分量的代表根；
        // size[root] 只对根有意义，表示该集合当前包含多少节点；
        // components 直接维护当前还剩多少个互不连通的集合。
        vector<int> parent, size;
        int components;

        explicit DSU(int n) : parent(n), size(n, 1), components(n) {
            // 初始没有处理任何城市间连接：每个节点自己就是一个根，也各自构成一个省份。
            iota(parent.begin(), parent.end(), 0);
        }

        int find(int x) {
            // 递归找到真正代表根，并把沿途节点直接改挂到根上。
            // 路径压缩不会改变“属于哪个集合”，只缩短以后 find 的父指针链。
            if (parent[x] != x) parent[x] = find(parent[x]);
            return parent[x];
        }

        void unite(int a, int b) {
            // 连通性只由代表根决定；先把两个普通节点都提升成各自集合的根。
            a = find(a);
            b = find(b);

            // 已经同根说明这条边只发生在同一省份内部，不会减少连通分量数量。
            if (a == b) return;

            // 按 size 把小树挂到大树下，避免父指针树长期退化得过深。
            if (size[a] < size[b]) swap(a, b);
            parent[b] = a;
            size[a] += size[b];

            // 只有两个不同集合真正合并时，连通分量数量才恰好减少 1。
            --components;
        }
    };

public:
    int findCircleNum(vector<vector<int>>& isConnected) {
        const int n = static_cast<int>(isConnected.size());
        DSU dsu(n);

        // isConnected 是对称矩阵，且对角线只是城市与自身相连；
        // 因此只扫描 j>i 的上三角即可，每条无向边只处理一次。
        for (int i = 0; i < n; ++i)
            for (int j = i + 1; j < n; ++j)
                if (isConnected[i][j]) dsu.unite(i, j);

        // 所有连接处理完后，DSU 的集合划分与真实省份一一对应。
        return dsu.components;
    }
};''',
}
