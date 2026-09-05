"""High-touch C++ implementation comment overrides.

These overrides keep algorithm code unchanged while adding learner-facing
comments next to the lines that carry the core invariant, state transition,
update order, or boundary semantics.

Add entries serially after reviewing each problem. `refined_data.py` merges this
layer after pedagogy overrides so `tools/gen_all.py` preserves the comments.
"""


CODE_COMMENT_OVERRIDES = {
    133: r'''// ---------- Solution ----------
class Solution {
public:
    Node* cloneGraph(Node* node) {
        if (!node) return nullptr;

        // cloneOf 同时承担两个角色：
        // 1) visited：key 已存在表示这个原节点已经被发现；
        // 2) identity map：value 是该原节点唯一对应的克隆对象。
        // 图中同一个节点可能从多条边再次到达，因此不能“每见一次就 new 一次”。
        unordered_map<Node*, Node*> cloneOf;

        // 起点必须先登记映射再入队。
        // 这样后续若沿环重新遇到起点，会直接复用这个克隆，而不会再创建第二个对象。
        cloneOf[node] = new Node(node->val);
        queue<Node*> pending;
        pending.push(node);

        while (!pending.empty()) {
            Node* original = pending.front();
            pending.pop();

            for (Node* neighbor : original->neighbors) {
                // 只有第一次发现 neighbor 时才创建克隆并安排后续遍历。
                // “先写 cloneOf，再入队”很重要：从这一行开始，neighbor 就已经被视为 visited；
                // 即使它在真正出队前又被另一条边遇到，也不会重复创建或重复入队。
                if (!cloneOf.count(neighbor)) {
                    cloneOf[neighbor] = new Node(neighbor->val);
                    pending.push(neighbor);
                }

                // 无论 neighbor 是第一次还是第 N 次被遇到，这条原图边都必须被复制。
                // 两端都通过 cloneOf 查找，保证共享邻居仍然指向同一个克隆对象。
                cloneOf[original]->neighbors.push_back(cloneOf[neighbor]);
            }
        }

        // 返回的是全新的克隆起点；它与原节点地址不同，但从该节点可到达的拓扑完全一致。
        return cloneOf[node];
    }
};''',

    200: r'''// ---------- Solution ----------
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int islands = 0;

        // 外层扫描只负责“发现一个此前没有被任何 BFS 覆盖的新连通分量”。
        // 因为每次 BFS 都会把整座岛染成 '0'，所以扫描时仍然是 '1' 的格子
        // 一定属于一座尚未计数的新岛。
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != '1') continue;

                // 只在“启动一次新的 BFS”时计数，而不是每看到一个陆地格就计数。
                // 一次 BFS 与一个四连通分量一一对应，因此这里正是 islands++ 的唯一时机。
                ++islands;
                queue<pair<int, int>> pending;

                // 起点入队时立即染色，相当于此刻就写入 visited。
                // 如果等到出队时才标记，起点或后续邻居可能在真正出队前
                // 被多个相邻格重复加入队列。
                pending.push({r, c});
                grid[r][c] = '0';

                while (!pending.empty()) {
                    auto [x, y] = pending.front();
                    pending.pop();

                    // grid 是隐式图：只枚举上下左右四条合法边；
                    // 对角线不是题目定义的连通关系，因此不能加入 directions。
                    for (auto& d : directions) {
                        int nx = x + d[0], ny = y + d[1];
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == '1') {
                            // 顺序必须是“先标记，再入队”。从标记这一刻起，该格就已失去再次入队的资格；
                            // 若先入队而暂不标记，同层其它格仍可能把它重复发现。
                            grid[nx][ny] = '0';
                            pending.push({nx, ny});
                        }
                    }
                }
            }
        }
        return islands;
    }
};''',
}
