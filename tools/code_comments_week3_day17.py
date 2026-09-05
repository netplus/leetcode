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

    684: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        // 节点编号是 1..n，因此下标 0 不参与算法，parent 需要开到 n+1。
        vector<int> parent(edges.size() + 1);
        iota(parent.begin(), parent.end(), 0);

        // find 返回当前连通分量代表根，并用路径压缩缩短后续查询链。
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };

        for (const auto& edge : edges) {
            // 一定要在把当前边加入 DSU 之前查询两端代表根。
            // 此时 a==b 等价于：仅靠此前已经处理的边，edge[0] 与 edge[1] 之间就已经存在路径。
            int a = find(edge[0]);
            int b = find(edge[1]);

            // 旧路径已经存在，再加入当前无向边就会闭合成环；当前边正是需要删除的冗余边。
            if (a == b) return edge;

            // 只有两端原本属于不同连通分量时才合并；这类边是在连接两个块，不会形成环。
            parent[a] = b;
        }

        // 题目保证输入由一棵树额外加入一条边得到，因此正常情况下不会走到这里。
        return {};
    }
};''',

    685: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        const int n = static_cast<int>(edges.size());

        // 第一阶段只处理“每个非根节点最多一个父亲”这条有根树结构约束。
        // directParent[v]==0 表示 v 还没见过父边；否则再次出现 -> v 就是唯一可能的二父节点。
        vector<int> directParent(n + 1, 0);
        int earlier = -1, later = -1;

        for (int i = 0; i < n; ++i) {
            int from = edges[i][0], to = edges[i][1];
            if (directParent[to] == 0) {
                directParent[to] = from;
            } else {
                // 当前 i 是较晚出现的第二条父边 later。
                // 再向前找到同样指向 to 的第一条父边 earlier；真正答案只可能在这两条边中。
                for (int j = 0; j < i; ++j) {
                    if (edges[j][1] == to) {
                        earlier = j;
                        break;
                    }
                }
                later = i;
                break;
            }
        }

        // 第二阶段复用 LC-684：把边方向暂时忽略，用 DSU 判断“剩余边是否还形成环”。
        vector<int> parent(n + 1);
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };

        for (int i = 0; i < n; ++i) {
            // 若存在二父冲突，先假设删除较晚的 later；因此重放图时必须跳过它。
            if (i == later) continue;

            int a = find(edges[i][0]);
            int b = find(edges[i][1]);

            if (a == b) {
                // 没有二父候选：问题退化为 LC-684，当前边就是唯一闭环边。
                // 有二父候选：later 已被跳过却仍成环，说明 later 不是答案；必须删 earlier。
                return earlier == -1 ? edges[i] : edges[earlier];
            }

            // 当前边没有成环，才把两个集合合并，维持“此前重放边的连通性”不变量。
            parent[b] = a;
        }

        // 能走到这里说明：跳过 later 后整张图没有环。
        // 因而删除 later 已经同时解决了二父问题并恢复有根树。
        return edges[later];
    }
};''',

    990: r'''// ---------- Solution ----------
class Solution {
public:
    bool equationsPossible(vector<string>& equations) {
        // 26 个小写字母各自先代表一个独立等价类；parent[root]==root 表示代表元。
        array<int, 26> parent;
        iota(parent.begin(), parent.end(), 0);

        // find 返回变量当前所属等价类的代表根，并通过路径压缩加速后续查询。
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };

        // 第一遍只处理所有 ==：必须先把直接与间接的相等关系全部闭包起来。
        // 不能遇到 != 就立即判断，因为后面的等式可能通过传递性再把两端合到同一集合。
        for (const string& equation : equations) {
            if (equation[1] == '=') {
                int a = find(equation[0] - 'a');
                int b = find(equation[3] - 'a');
                parent[a] = b;
            }
        }

        // 第二遍才检查 !=：此时同根意味着等式闭包已经强制两变量必须取相同值，
        // 若当前约束又要求不等，就产生不可满足的逻辑矛盾。
        for (const string& equation : equations) {
            if (equation[1] == '!' && find(equation[0] - 'a') == find(equation[3] - 'a')) {
                // x!=x 也会自然落入这里，因为任意变量始终与自己同根。
                return false;
            }
        }

        // 所有 != 都跨不同等价类时，可给不同根分配不同整数，从而同时满足全部约束。
        return true;
    }
};''',
}
