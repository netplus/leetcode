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
// 解法精讲｜图的 BFS 深拷贝：原节点到克隆节点映射
// - 核心要点：
//   1. 思路起点：图可能有环，必须用哈希表同时充当 visited 和克隆索引；第一次见原节点时创建克隆，随后连接克隆边。
//   2. 执行逻辑：1. 空输入返回空；2. 为起点建克隆并入队；3. 遍历每条邻接边：必要时建邻居克隆，再把它接到当前克隆。
//   3. 为什么这样做：映射保证每个原节点只创建一个克隆；遍历原图每条邻接关系时在对应克隆间加入同样关系，因此节点值和拓扑均一致且地址独立。
// - 边界与易错点：不能按 val 当唯一键来依赖题外假设，直接按 Node* 映射更通用；先登记再入队可处理环和自引用。
// - 举一反三：对象图、带随机指针链表和 DAG 复制都使用“身份映射 + 遍历重建引用”的两阶段逻辑。
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

