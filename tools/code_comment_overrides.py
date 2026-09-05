"""High-touch C++ implementation comment overrides.

These overrides keep algorithm code unchanged while adding learner-facing
comments next to the lines that carry the core invariant, state transition,
update order, or boundary semantics.

Add entries serially after reviewing each problem. `refined_data.py` merges this
layer after pedagogy overrides so `tools/gen_all.py` preserves the comments.
"""


CODE_COMMENT_OVERRIDES = {
    1: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> twoSum(vector<int>& nums, int target) {
        // indexOf 只保存“已经扫描过”的历史元素：value -> 某个历史下标。
        // 因此在处理 i 时，表中的下标都严格小于 i；这也是后面命中时
        // 能保证两个下标不同的关键不变量。
        unordered_map<int, int> indexOf;
        indexOf.reserve(nums.size() * 2); // 仅减少扩容/rehash，不影响算法正确性

        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            // 一旦当前值 nums[i] 固定，另一个值并不是“任意候选”，
            // 而是被方程 nums[i] + old = target 唯一确定为 target-nums[i]。
            // 哈希优化省掉的正是暴力算法里反复线性寻找这个确定值的工作。
            const int complement = target - nums[i];

            // 这里只查询历史，不希望查询动作产生任何新状态，所以使用 find()。
            // 若命中，it->second 一定来自某个历史位置 < i；当前 i 尚未写入表中，
            // 因而 return 的两个下标天然不同，不会把同一个元素使用两次。
            auto it = indexOf.find(complement);
            if (it != indexOf.end()) {
                return {it->second, i};
            }

            // 必须“先查补数，再记录当前值”。
            // 若先插入 nums[i]，当 target == 2*nums[i] 时，当前元素可能立刻查到自己，
            // 违反题目“不能使用两次同一个元素”的要求。
            indexOf[nums[i]] = i;
        }
        return {};  // 题目保证有唯一解；保留兜底使本地接口更健壮
    }
};''',

    53: r'''// ---------- Solution ----------
class Solution {
public:
    int maxSubArray(vector<int>& nums) {
        // current：必须以“当前扫描位置”结尾的最大子数组和。
        // best：截至当前，所有可能终点中出现过的最大子数组和。
        // 两者都从 nums[0] 开始，而不能从 0 开始：题目要求子数组非空；
        // 若数组全为负数，0 代表的是一个并不存在的空子数组，会得到错误答案。
        int current = nums[0];
        int best = nums[0];

        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            // 所有以 i 结尾的合法区间只有两类来源：
            // 1) 从 nums[i] 重新开始；
            // 2) 把 nums[i] 接到某个以 i-1 结尾的区间后面。
            //
            // 第二类中只需要保留上一位置最大的 current：所有候选以后都会追加
            // 完全相同的未来后缀，当前和更小的候选永远不可能反超更大的候选，
            // 因而可以永久淘汰。这就是这里能把 O(n) 个结尾候选压成 1 个状态的原因。
            current = max(nums[i], current + nums[i]);

            // current 只回答“必须在 i 结束时最好是多少”；它下一步可能下降。
            // best 保存历史全局答案，因此必须独立更新，不能直接用最新 current 代替。
            best = max(best, current);
        }
        return best;
    }
};''',

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

    207: r'''// ---------- Solution ----------
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        // next[u] 保存“完成 u 之后会释放哪些后修课程”；
        // indegree[v] 保存 v 当前还剩多少条尚未被删除的先修依赖。
        vector<vector<int>> next(numCourses);
        vector<int> indegree(numCourses, 0);

        for (const auto& edge : prerequisites) {
            // 题目给 [a,b] 的语义是“先修 b，才能学习 a”，
            // 所以有向边必须建成 b -> a，而不是 a -> b。
            // 这条边也意味着 a 多了一项尚未满足的前置依赖，因此 indegree[a]++。
            next[edge[1]].push_back(edge[0]);
            ++indegree[edge[0]];
        }

        // ready 中始终只放“当前剩余入度为 0”的课程：
        // 它们已经没有未完成的先修课，可以作为合法的下一步。
        // 没有任何依赖的孤立课程也必须在这里进入队列，否则会漏计课程。
        queue<int> ready;
        for (int course = 0; course < numCourses; ++course) {
            if (indegree[course] == 0) ready.push(course);
        }

        // completed 不是 BFS 层数，而是已经从剩余依赖图中合法删除的节点数。
        int completed = 0;
        while (!ready.empty()) {
            int course = ready.front();
            ready.pop();
            ++completed;

            // 完成 course 等价于从“剩余图”中删掉 course 以及它的所有出边。
            // 因此每条 course -> dependent 都让 dependent 少一个未满足先修条件。
            for (int dependent : next[course]) {
                // 只有入度恰好从 1 降到 0 的这一刻才入队。
                // 更早时仍有先修没完成；更晚再入队会造成同一课程重复处理。
                if (--indegree[dependent] == 0) ready.push(dependent);
            }
        }

        // DAG 会持续产生零入度节点，最终删完全部课程；
        // 若 completed 不足，剩余子图没有零入度节点，说明其中存在有向环互相等待。
        return completed == numCourses;
    }
};''',

    695: r'''// ---------- Solution ----------
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int best = 0;

        // 与 LC-200 一样，外层扫描只在遇到“尚未被染色的新陆地”时启动一次 BFS。
        // 不同点是：这里不统计分量个数，而是要统计每个分量包含多少个陆地格。
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != 1) continue;

                // area 只描述“当前这一座岛”的面积；每发现新分量都必须重新从 0 开始。
                int area = 0;
                queue<pair<int, int>> pending;

                // 仍然采用“入队即标记”。这样 pending 中的每个格子都已经唯一占有一次处理资格，
                // 后续不会被其它邻居重复加入队列。
                pending.push({r, c});
                grid[r][c] = 0;

                while (!pending.empty()) {
                    auto [x, y] = pending.front();
                    pending.pop();

                    // 面积统一在“格子正式出队处理”时 +1。
                    // 每个格子只会出队一次，因此每块陆地恰好贡献 1；
                    // 不要在发现邻居时也计数，否则同一个格会被重复计算。
                    ++area;

                    for (auto& d : directions) {
                        int nx = x + d[0], ny = y + d[1];
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == 1) {
                            // 先染色再入队，继续保持“一格只入队一次”的 visited 不变量。
                            grid[nx][ny] = 0;
                            pending.push({nx, ny});
                        }
                    }
                }

                // 队列清空后，area 才代表一个完整连通分量的面积。
                // 此时再和历史最大值比较，避免把尚未遍历完整的局部面积当成最终候选。
                best = max(best, area);
            }
        }
        return best;
    }
};''',
}
