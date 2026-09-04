// ============================================================================
// LC-133：克隆图
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 15 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你无向连通图中一个节点的引用，请你返回该图的深拷贝 （克隆）。
// 图中的每个节点都包含它的值 val （int） 和其邻居的列表（list[Node]）。
// class Node { public int val; public List<Node> neighbors; } 测试用例格式：简单起见，每个节点的值都和它的索引相同。
// 例如，第一个节点值为 1（val = 1），第二个节点值为 2（val = 2），以此类推。
// 该图在测试用例中使用邻接列表表示。
// 邻接列表是用于表示有限图的无序列表的集合。
// 每个列表都描述了图中节点的邻居集。
// 给定节点将始终是图中的第一个节点（值为 1）。
// 你必须将给定节点的拷贝作为对克隆图的引用返回。
//
// 约束与要求：
//   - 这张图中的节点数在 [0, 100] 之间。
//   - 1 <= Node.val <= 100
//   - 每个节点值 Node.val 都是唯一的，
//   - 图中没有重复的边，也没有自环。
//   - 图是连通图，你可以从给定节点访问到所有节点。
//
// 复杂度目标：O(V+E) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜图的深拷贝：遍历负责发现结构，old->new 映射负责保持对象身份
//
// 1. 图像直觉
//   原图：
//   A ----- B
//   |       |
//   +--- C -+
//
//   如果从 A->B 创建一个 B'，又从 C->B 再创建一个 B''，得到的就不是原图的深拷贝，因为原图只有一个共享节点 B。
//
//   所以第一次见到节点时就登记：
//   cloneOf[A] = A'
//   cloneOf[B] = B'
//   cloneOf[C] = C'
//
//   之后无论从哪条边再次遇到 B，都只引用同一个 cloneOf[B]。
//
//   遍历一条原边 A -> B 时，克隆图只做：
//   A'->neighbors.push_back(B')。
//
// 2. 一句话核心
//   每个原节点第一次出现时只创建一个克隆并登记映射；以后遍历原图边时，只用映射找到两端克隆并重建同样的邻接关系。
//
// 3. 公式 / 不变量
//   映射不变量：
//   cloneOf[x] 已存在 <=> 原节点 x 已被发现
//   cloneOf[x] 是 x 唯一对应的克隆对象
//
//   处理原边 original -> neighbor：
//   若 neighbor 未登记：
//       cloneOf[neighbor] = new Node(neighbor->val)
//       neighbor 入队
//   然后始终执行：
//   cloneOf[original]->neighbors.push_back(cloneOf[neighbor])
//
// 4. 执行步骤
//   1. 空输入直接返回 nullptr；否则先创建起点克隆，立刻写入 cloneOf，再把原起点入队
//   2. BFS 每次弹出一个 original，遍历它的全部 neighbors
//   3. 若某 neighbor 尚未出现在 cloneOf，立即创建其克隆、登记映射并把原 neighbor 入队
//   4. 无论 neighbor 是否第一次出现，都把 cloneOf[neighbor] 接入 cloneOf[original] 的 neighbors
//   5. 遍历结束返回 cloneOf[node]；原图地址与克隆图地址完全独立，但拓扑关系保持一致
//   记忆：图可以绕回来，所以不能见一次 new 一次；一个原对象永远映射到同一个新对象。
//
// 5. 为什么不会漏 / 不会重
//   每个原节点只在首次发现时创建一次克隆，因此不会因环或多条入边产生重复对象。每条原邻接关系在处理其起点邻接表时都会在对应克隆节点间重建，所以不会漏边；映射始终使用原节点指针作为身份键，确保共享关系保持不变。
//
// 6. 边界与易错点
//   关键是先登记映射再入队，否则环可能导致重复创建。不要把 node->val 当作通用对象身份；本题虽保证 val 唯一，但按 Node* 建映射才是可迁移的深拷贝模型。哈希表不是额外的 visited 再加一张 clone 表，而是一张表同时完成两件事。
//
// 7. 举一反三
//   它和带 random 指针链表、对象依赖图、AST/IR 克隆本质相同：先建立 old->new 身份映射，再按原引用关系重建新引用。LC-200 的 visited 只回答“见过没有”，本题的映射进一步回答“见过后对应哪个新对象”。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n (节点数，编号为 1..n) m (边数)。
//   接下来 m 行：u v (u 与 v 之间的边)。
//   输出克隆图的邻接表：对每个节点 i，以空格分隔其所有邻居，
//   每行对应一个节点。
// test.in 的预期输出：2 | 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;

class Node {
public:
    int val;
    vector<Node*> neighbors;
    Node() { val = 0; neighbors = vector<Node*>(); }
    Node(int _val) { val = _val; neighbors = vector<Node*>(); }
    Node(int _val, vector<Node*> _neighbors) { val = _val; neighbors = _neighbors; }
};

// ---------- 题解实现 ----------
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;
        unordered_map<Node*, Node*> cloneOf;
        cloneOf[node] = new Node(node->val);
        queue<Node*> pending;
        pending.push(node);

        while (!pending.empty()) {
            Node* original = pending.front();
            pending.pop();
            for (Node* neighbor : original->neighbors) {
                if (!cloneOf.count(neighbor)) {
                    cloneOf[neighbor] = new Node(neighbor->val);
                    pending.push(neighbor);
                }
                cloneOf[original]->neighbors.push_back(cloneOf[neighbor]);
            }
        }
        return cloneOf[node];
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, m; if (!(cin >> n >> m)) return 0;
    vector<Node*> nodes(n + 1);
    for (int i = 1; i <= n; ++i) nodes[i] = new Node(i);
    for (int e = 0; e < m; ++e) {
        int u, v; cin >> u >> v;
        nodes[u]->neighbors.push_back(nodes[v]);
        nodes[v]->neighbors.push_back(nodes[u]);
    }
    Node* src = (n >= 1) ? nodes[1] : nullptr;
    Node* cl = sol.cloneGraph(src);
    // Print adjacency of the clone via BFS from cl (val == 1).
    if (!cl) { for (int i = 0; i < n; ++i) cout << "\n"; return 0; }
    unordered_map<int, Node*> seen; seen[cl->val] = cl;
    queue<Node*> q; q.push(cl);
    vector<vector<int>> adj(n + 1);
    while (!q.empty()) {
        Node* c = q.front(); q.pop();
        for (Node* nb : c->neighbors) {
            adj[c->val].push_back(nb->val);
            if (!seen.count(nb->val)) { seen[nb->val] = nb; q.push(nb); }
        }
    }
    for (int i = 1; i <= n; ++i) {
        sort(adj[i].begin(), adj[i].end());
        for (size_t j = 0; j < adj[i].size(); ++j) cout << adj[i][j] << " \n"[j + 1 == adj[i].size()];
        if (adj[i].empty()) cout << "\n";
    }
    return 0;
}
