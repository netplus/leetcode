"""Week 3 reviewed explanations and C++ implementations."""

REFINEMENTS = {}


def add(num, pattern, model, steps, proof, pitfalls, transfer, code):
    key_points = [
        model,
        "；".join(f"{i}. {step}" for i, step in enumerate(steps, 1)) + "。",
        proof,
    ]
    REFINEMENTS[num] = {
        "pattern": pattern,
        "key_points": key_points,
        "model": model,
        "steps": steps,
        "proof": proof,
        "pitfalls": pitfalls,
        "transfer": transfer,
        "code": code,
    }


add(200, "网格连通分量：BFS 染色",
    "每个岛屿是上下左右相邻的 '1' 构成的连通分量；发现未访问陆地时答案加一，并一次淹没整个分量。",
    ["扫描所有格子", "遇到 '1' 就入队并立即标为 '0'", "BFS 扩展四邻域直到该岛全部处理"],
    "每次启动 BFS 的格子不属于此前任何分量；BFS 恰访问与它连通的全部陆地并标记，故每个岛只计数一次且无遗漏。",
    "必须在入队时标记，避免同一格重复入队；只允许四方向而非对角线；空网格应直接返回 0。",
    "网格岛屿、区域填充、连通块面积与封闭区域都共享“扫描起点 + 洪泛标记”框架。",
r'''// ---------- Solution ----------
class Solution {
public:
    int numIslands(vector<vector<char>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int islands = 0;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != '1') continue;
                ++islands;
                queue<pair<int, int>> pending;
                pending.push({r, c});
                grid[r][c] = '0';  // 入队即标记，杜绝重复入队
                while (!pending.empty()) {
                    auto [x, y] = pending.front();
                    pending.pop();
                    for (auto& d : directions) {
                        int nx = x + d[0], ny = y + d[1];
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == '1') {
                            grid[nx][ny] = '0';
                            pending.push({nx, ny});
                        }
                    }
                }
            }
        }
        return islands;
    }
};''')


add(695, "网格 BFS：累计连通分量面积",
    "与岛屿计数相同，但每次洪泛时累计弹出的陆地格数，并用它更新最大面积。",
    ["扫描未访问的 1", "BFS 入队染色并令 area 逐格加一", "一个分量结束后更新 best"],
    "BFS 访问集合恰是起点所在四连通分量，每格贡献一次，所以 area 是该岛面积；对所有分量取最大即答案。",
    "全水域答案为 0；入队即改为 0；若调用方要求保留 grid，应改用独立 visited 数组。",
    "把分量内的累加器替换为周长、颜色频次或边界框，就能解决一族网格聚合问题。",
r'''// ---------- Solution ----------
class Solution {
public:
    int maxAreaOfIsland(vector<vector<int>>& grid) {
        if (grid.empty() || grid[0].empty()) return 0;
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int best = 0;

        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] != 1) continue;
                int area = 0;
                queue<pair<int, int>> pending;
                pending.push({r, c});
                grid[r][c] = 0;
                while (!pending.empty()) {
                    auto [x, y] = pending.front();
                    pending.pop();
                    ++area;
                    for (auto& d : directions) {
                        int nx = x + d[0], ny = y + d[1];
                        if (nx >= 0 && nx < rows && ny >= 0 && ny < cols && grid[nx][ny] == 1) {
                            grid[nx][ny] = 0;
                            pending.push({nx, ny});
                        }
                    }
                }
                best = max(best, area);
            }
        }
        return best;
    }
};''')


add(133, "图的 BFS 深拷贝：原节点到克隆节点映射",
    "图可能有环，必须用哈希表同时充当 visited 和克隆索引；第一次见原节点时创建克隆，随后连接克隆边。",
    ["空输入返回空", "为起点建克隆并入队", "遍历每条邻接边：必要时建邻居克隆，再把它接到当前克隆"],
    "映射保证每个原节点只创建一个克隆；遍历原图每条邻接关系时在对应克隆间加入同样关系，因此节点值和拓扑均一致且地址独立。",
    "不能按 val 当唯一键来依赖题外假设，直接按 Node* 映射更通用；先登记再入队可处理环和自引用。",
    "对象图、带随机指针链表和 DAG 复制都使用“身份映射 + 遍历重建引用”的两阶段逻辑。",
r'''// ---------- Solution ----------
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
};''')


add(207, "Kahn 拓扑排序：逐步删除入度为零节点",
    "先修关系 b->a 构成有向图；若能反复选出当前无依赖课程并删除其出边，最终处理全部节点则无环。",
    ["建立邻接表和每门课入度", "所有入度 0 课程入队", "出队计数并降低后继入度，新变 0 时入队"],
    "入队课程的所有先修均已处理，所以选择合法；有向无环图必至少有一个入度 0 节点，若处理数不足则剩余子图必含环。",
    "边方向应从先修课指向后修课；重复入队通过仅在入度降到 0 时加入来避免；孤立课程也应计数。",
    "构建顺序、任务依赖、软件包安装和流水线调度都可以拓扑排序检测环并产生执行序列。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool canFinish(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> next(numCourses);
        vector<int> indegree(numCourses, 0);
        for (const auto& edge : prerequisites) {
            next[edge[1]].push_back(edge[0]);
            ++indegree[edge[0]];
        }
        queue<int> ready;
        for (int course = 0; course < numCourses; ++course) {
            if (indegree[course] == 0) ready.push(course);
        }

        int completed = 0;
        while (!ready.empty()) {
            int course = ready.front();
            ready.pop();
            ++completed;
            for (int dependent : next[course]) {
                if (--indegree[dependent] == 0) ready.push(dependent);
            }
        }
        return completed == numCourses;
    }
};''')


add(994, "多源 BFS：按分钟同步扩散",
    "所有初始腐烂橘子在时间 0 同时作为 BFS 源；一层扩展对应一分钟，新腐烂橘子进入下一层。",
    ["统计新鲜橘子并把所有腐烂橘子入队", "每轮固定当前队列大小并令 minutes++", "感染四邻新鲜橘子、减少 fresh；最后判断是否清零"],
    "多源 BFS 首次到达某格的层数就是它到最近初始腐烂橘子的最短距离；逐层并行扩散与题目时间过程等价。",
    "没有新鲜橘子应返回 0；隔离的新鲜橘子最终返回 -1；分钟只在 fresh>0 且确有当前层时增加。",
    "火灾传播、最近设施距离和多个起点的无权最短路都可把所有源同时入队。",
r'''// ---------- Solution ----------
class Solution {
public:
    int orangesRotting(vector<vector<int>>& grid) {
        const int rows = static_cast<int>(grid.size());
        const int cols = static_cast<int>(grid[0].size());
        queue<pair<int, int>> rotten;
        int fresh = 0;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (grid[r][c] == 2) rotten.push({r, c});
                else if (grid[r][c] == 1) ++fresh;
            }
        }

        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int minutes = 0;
        while (!rotten.empty() && fresh > 0) {
            int layer = static_cast<int>(rotten.size());
            ++minutes;
            while (layer--) {
                auto [r, c] = rotten.front();
                rotten.pop();
                for (auto& d : directions) {
                    int nr = r + d[0], nc = c + d[1];
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && grid[nr][nc] == 1) {
                        grid[nr][nc] = 2;
                        --fresh;
                        rotten.push({nr, nc});
                    }
                }
            }
        }
        return fresh == 0 ? minutes : -1;
    }
};''')


add(127, "单词隐式图上的 BFS",
    "每个单词是节点，相差一个字符的单词间有边；无需显式 O(N^2) 建图，可逐位替换 26 个字母并在字典集合中查邻居。",
    ["若 endWord 不在字典返回 0", "beginWord 以距离 1 入队", "逐层枚举每个位置的替换词，未访问词删除集合后入队"],
    "BFS 按边数递增访问节点，首次生成 endWord 的路径必最短；从集合删除等价于标记 visited，不会影响任何更短路径。",
    "序列长度包含起点和终点；beginWord 无需在 wordList；替换后要恢复原字符。",
    "状态可局部变换且每步代价相同时，可把问题视为隐式无权图，用 BFS 找最少操作数。",
r'''// ---------- Solution ----------
class Solution {
public:
    int ladderLength(string beginWord, string endWord, vector<string>& wordList) {
        unordered_set<string> unused(wordList.begin(), wordList.end());
        if (!unused.count(endWord)) return 0;
        unused.erase(beginWord);
        queue<pair<string, int>> pending;
        pending.push({beginWord, 1});

        while (!pending.empty()) {
            auto [word, distance] = pending.front();
            pending.pop();
            if (word == endWord) return distance;
            for (int i = 0; i < static_cast<int>(word.size()); ++i) {
                const char original = word[i];
                for (char c = 'a'; c <= 'z'; ++c) {
                    if (c == original) continue;
                    word[i] = c;
                    auto it = unused.find(word);
                    if (it != unused.end()) {
                        pending.push({word, distance + 1});
                        unused.erase(it);  // 入队即标记，避免重复搜索
                    }
                }
                word[i] = original;
            }
        }
        return 0;
    }
};''')


add(542, "多源 BFS：从所有 0 反向扩散距离",
    "若从每个 1 单独寻找最近 0 会重复搜索；把所有 0 同时作为源，首次到达每个 1 时就是其最短曼哈顿距离。",
    ["0 的距离设为 0 并全部入队，1 先设为 -1", "弹出格子并访问四邻", "只给尚未访问格赋 current+1 并入队"],
    "BFS 队列按距离非降序处理，多源可视为连接到虚拟超级源；因此未访问邻居的首次赋值必为到任一 0 的最短距离。",
    "需要独立未访问标记或复用结果中的 -1；所有 0 必须同时入队；四向距离不是对角线距离。",
    "“求每点到最近某类节点”通常反向从目标集合做多源 BFS，比逐点搜索高效。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> updateMatrix(vector<vector<int>>& mat) {
        const int rows = static_cast<int>(mat.size());
        const int cols = static_cast<int>(mat[0].size());
        vector<vector<int>> distance(rows, vector<int>(cols, -1));
        queue<pair<int, int>> pending;
        for (int r = 0; r < rows; ++r) {
            for (int c = 0; c < cols; ++c) {
                if (mat[r][c] == 0) {
                    distance[r][c] = 0;
                    pending.push({r, c});
                }
            }
        }

        const int directions[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        while (!pending.empty()) {
            auto [r, c] = pending.front();
            pending.pop();
            for (auto& d : directions) {
                int nr = r + d[0], nc = c + d[1];
                if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && distance[nr][nc] == -1) {
                    distance[nr][nc] = distance[r][c] + 1;
                    pending.push({nr, nc});
                }
            }
        }
        return distance;
    }
};''')


add(417, "反向洪泛：从海岸寻找可逆到达点",
    "正向从每格向不高于自己的邻格流水代价高；反向从海岸出发，只走到高度不低的邻格，得到能流向该海洋的所有格。",
    ["分别把太平洋边界和大西洋边界作为多源起点", "反向 BFS 只允许 nextHeight>=currentHeight", "取两份可达标记的交集"],
    "反向边恰是原流水边的逆边，所以从海岸反向可达当且仅当该格正向能流到海岸；同时在两集合中即能到两洋。",
    "四条边的角落可能重复入队，需先检查 visited；两个海洋必须使用独立标记；相等高度允许流动。",
    "当终点集合很小而起点很多时，反转图从终点做多源搜索常能消除重复工作。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> pacificAtlantic(vector<vector<int>>& heights) {
        const int rows = static_cast<int>(heights.size());
        const int cols = static_cast<int>(heights[0].size());
        vector<vector<char>> pacific(rows, vector<char>(cols, false));
        vector<vector<char>> atlantic(rows, vector<char>(cols, false));

        auto flood = [&](vector<vector<char>>& seen, vector<pair<int, int>> starts) {
            queue<pair<int, int>> pending;
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
                    if (nr >= 0 && nr < rows && nc >= 0 && nc < cols && !seen[nr][nc]
                        && heights[nr][nc] >= heights[r][c]) {
                        seen[nr][nc] = true;
                        pending.push({nr, nc});
                    }
                }
            }
        };

        vector<pair<int, int>> pStarts, aStarts;
        for (int r = 0; r < rows; ++r) {
            pStarts.push_back({r, 0});
            aStarts.push_back({r, cols - 1});
        }
        for (int c = 0; c < cols; ++c) {
            pStarts.push_back({0, c});
            aStarts.push_back({rows - 1, c});
        }
        flood(pacific, move(pStarts));
        flood(atlantic, move(aStarts));

        vector<vector<int>> answer;
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (pacific[r][c] && atlantic[r][c]) answer.push_back({r, c});
        return answer;
    }
};''')


add(547, "并查集：动态合并连通关系",
    "每个城市初始自成集合；矩阵中的连接把两个集合合并，最终根节点数量就是省份数。",
    ["初始化 parent、size 和 components=n", "只扫描矩阵上三角的连接", "find 路径压缩，union 按大小合并并令 components--"],
    "并查集等价类始终与已处理边的连通分量一致；合并不同根恰使分量数减一，同根边不改变分量。",
    "矩阵对称且对角线为 1，只扫 j>i 可避免重复；components 只在真正合并时减少。",
    "并查集适合离线连通性、冗余边、账户合并和 Kruskal 最小生成树。",
r'''// ---------- Solution ----------
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
};''')


add(684, "并查集检测无向图首条成环边",
    "树加入一条额外边后恰有一个环；按输入顺序加边，若某边两端已在同一集合，它就是使路径闭合的冗余边。",
    ["初始化每个节点为独立集合", "依次 find 两端", "根相同则返回当前边，否则合并"],
    "在当前边之前，并查集准确表示已有边的连通性；同根说明已有路径连接两端，再加当前边必成环，异根则不会成环。",
    "节点编号是 1..n；题目要求返回输入中最后可删的合法边，而单额外边场景下按序检测到的成环边正满足。",
    "在线加边判环、网络布线和 Kruskal 跳过成环边都使用同一 DSU 判定。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findRedundantConnection(vector<vector<int>>& edges) {
        vector<int> parent(edges.size() + 1);
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        for (const auto& edge : edges) {
            int a = find(edge[0]), b = find(edge[1]);
            if (a == b) return edge;
            parent[a] = b;
        }
        return {};
    }
};''')


add(990, "并查集处理等价与不等约束",
    "等式具有传递性，先把所有 a==b 合并为等价类；再检查每条 a!=b 是否错误地落在同一类。",
    ["初始化 26 个字母集合", "第一遍只合并 ==", "第二遍检查 != 的两个根是否相同"],
    "所有等式的传递闭包由并查集表示；若不等式两端同根则约束矛盾，若所有不等式跨集合，可给各集合赋不同值满足全部条件。",
    "必须先统一处理等式，否则按输入顺序检查不等式可能过早得出结论；x!=x 必然失败。",
    "等价类归并后再验证互斥约束，是符号约束、别名分析和账户身份归一化的常见套路。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool equationsPossible(vector<string>& equations) {
        array<int, 26> parent;
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        for (const string& equation : equations) {
            if (equation[1] == '=') {
                int a = find(equation[0] - 'a');
                int b = find(equation[3] - 'a');
                parent[a] = b;
            }
        }
        for (const string& equation : equations) {
            if (equation[1] == '!' && find(equation[0] - 'a') == find(equation[3] - 'a')) {
                return false;
            }
        }
        return true;
    }
};''')


add(685, "有向树冗余边：二父节点与有向环分类",
    "非法结构只有两类症状：某节点入度为 2、或存在环；先记录造成二父的两条候选边，再跳过较晚边做并查集判环。",
    ["扫描父节点记录，得到 earlier/later 候选", "并查集重放所有边但跳过 later", "若仍成环返回 earlier；无环返回 later；无二父时返回成环边"],
    "跳过 later 后若无环，其删除即可恢复每点单父的树；若仍有环，环必包含 earlier，删除它才能同时消除环和二父；无二父时唯一问题就是环。",
    "这是有向问题，不能直接套无向冗余边；候选需保留输入顺序；并查集只用于检测忽略方向后的环。",
    "复杂结构题常先按违反的局部约束分类，再用一个简化判定器验证候选，而非一次性硬写所有分支。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findRedundantDirectedConnection(vector<vector<int>>& edges) {
        const int n = static_cast<int>(edges.size());
        vector<int> directParent(n + 1, 0);
        int earlier = -1, later = -1;
        for (int i = 0; i < n; ++i) {
            int from = edges[i][0], to = edges[i][1];
            if (directParent[to] == 0) {
                directParent[to] = from;
            } else {
                for (int j = 0; j < i; ++j) {
                    if (edges[j][1] == to) { earlier = j; break; }
                }
                later = i;
                break;
            }
        }

        vector<int> parent(n + 1);
        iota(parent.begin(), parent.end(), 0);
        function<int(int)> find = [&](int x) {
            return parent[x] == x ? x : parent[x] = find(parent[x]);
        };
        for (int i = 0; i < n; ++i) {
            if (i == later) continue;
            int a = find(edges[i][0]), b = find(edges[i][1]);
            if (a == b) {
                return earlier == -1 ? edges[i] : edges[earlier];
            }
            parent[b] = a;
        }
        return edges[later];
    }
};''')


add(70, "线性 DP：斐波那契状态压缩",
    "到第 i 阶的最后一步只能来自 i-1 或 i-2，因此 ways[i]=ways[i-1]+ways[i-2]。",
    ["设置 ways(1)=1、ways(2)=2", "用两个变量滚动到 n", "每步计算 next 后整体前移"],
    "两种最后一步互斥且覆盖全部走法；若较小台阶计数正确，相加就得到当前台阶全部方案，归纳成立。",
    "n=1 需直接返回；状态含义是“到达第 i 阶”而非剩余步数；官方 n 范围使 int 足够。",
    "只依赖固定前几项的 DP 都可用滚动变量压缩，如铺砖、解码和线性递推序列。",
r'''// ---------- Solution ----------
class Solution {
public:
    int climbStairs(int n) {
        if (n <= 2) return n;
        int previous2 = 1, previous1 = 2;
        for (int step = 3; step <= n; ++step) {
            int current = previous1 + previous2;
            previous2 = previous1;
            previous1 = current;
        }
        return previous1;
    }
};''')


add(198, "打家劫舍线性 DP：选或不选",
    "处理当前房屋时，最优解要么不偷它并保持前一最优，要么偷它并加上前两间以前的最优。",
    ["previous2 表示 dp[i-2]，previous1 表示 dp[i-1]", "current=max(previous1,previous2+money)", "滚动更新两个状态"],
    "任一合法方案按是否包含当前房屋分成互斥两类；两类最优分别由两个转移项给出，取最大覆盖全部可能。",
    "滚动更新顺序不能覆盖旧 previous1；允许一间也不偷时空前缀为 0；金额非负。",
    "路径图上的“不选相邻点”是独立集 DP，删点后可扩展到环、树和带冷却时间的选择问题。",
r'''// ---------- Solution ----------
class Solution {
public:
    int rob(vector<int>& nums) {
        int previous2 = 0, previous1 = 0;
        for (int money : nums) {
            int current = max(previous1, previous2 + money);
            previous2 = previous1;
            previous1 = current;
        }
        return previous1;
    }
};''')


add(213, "环形 DP：拆成两个互斥线性区间",
    "首尾相邻，任何合法方案不可能同时选二者；完整最优必属于“不选尾”或“不选首”两类之一。",
    ["单元素直接返回", "分别求区间 [0,n-2] 与 [1,n-1] 的线性打家劫舍", "返回两者最大值"],
    "两种区间覆盖所有合法方案：若选首则一定不选尾，属于第一类；若不选首则属于第二类。每类内部由线性 DP 精确求优。",
    "n=1 时两个区间会无效，必须特判；区间端点采用闭区间并保持一致。",
    "环形约束常通过枚举首元素少量状态来断环，例如环形染色、环上相邻选择和状态机 DP。",
r'''// ---------- Solution ----------
class Solution {
    static int robRange(const vector<int>& nums, int left, int right) {
        int previous2 = 0, previous1 = 0;
        for (int i = left; i <= right; ++i) {
            int current = max(previous1, previous2 + nums[i]);
            previous2 = previous1;
            previous1 = current;
        }
        return previous1;
    }

public:
    int rob(vector<int>& nums) {
        if (nums.size() == 1) return nums[0];
        return max(robRange(nums, 0, static_cast<int>(nums.size()) - 2),
                   robRange(nums, 1, static_cast<int>(nums.size()) - 1));
    }
};''')


add(746, "线性 DP：到达台阶的最低成本",
    "付费发生在离开当前台阶；到达下一位置时，可从前一阶或前两阶跳来，取累计成本更小的一条。",
    ["previous2/previous1 表示到达前两个位置的最低成本", "对 i=2..n 计算 min(previous1+cost[i-1],previous2+cost[i-2])", "dp[n] 即越过楼顶的成本"],
    "到达 i 的最后一跳只可能从 i-1 或 i-2，且相应必须支付出发台阶费用；两类覆盖全部路径并可取各自最优。",
    "可从 0 或 1 开始，所以初始到达成本都为 0；目标位置是 n 而不是 n-1。",
    "“把动作成本放在边上”能帮助澄清路径 DP 的状态与转移，适用于最小跳跃代价和网格路径。",
r'''// ---------- Solution ----------
class Solution {
public:
    int minCostClimbingStairs(vector<int>& cost) {
        int previous2 = 0, previous1 = 0;
        for (int i = 2; i <= static_cast<int>(cost.size()); ++i) {
            int current = min(previous1 + cost[i - 1], previous2 + cost[i - 2]);
            previous2 = previous1;
            previous1 = current;
        }
        return previous1;
    }
};''')


add(62, "网格路径 DP：二维依赖压缩为一维",
    "到达格子 (r,c) 只能从上方或左方；一维 dp[c] 在更新前表示上方路径数，dp[c-1] 表示本行左方路径数。",
    ["第一行所有 dp 初始化为 1", "逐行从 c=1 更新 dp[c]+=dp[c-1]", "最后一列即终点方案数"],
    "更新顺序从左到右确保两个转移来源分别仍是旧上方与新左方；按行归纳后 dp 精确表示当前行各格路径数。",
    "边界第一行/列只有一种走法；一维压缩时遍历方向由依赖决定；官方保证答案不超过 2e9。",
    "网格 DP 的空间压缩要画出依赖箭头；若依赖右下方，遍历方向需相应反转。",
r'''// ---------- Solution ----------
class Solution {
public:
    int uniquePaths(int m, int n) {
        vector<int> dp(n, 1);
        for (int row = 1; row < m; ++row) {
            for (int col = 1; col < n; ++col) {
                dp[col] += dp[col - 1];
            }
        }
        return dp[n - 1];
    }
};''')


add(64, "网格最短路径 DP：滚动一行",
    "dp[c] 表示到当前行第 c 列的最小路径和；更新时从上方旧 dp[c] 与左方新 dp[c-1] 中取较小者再加当前格。",
    ["dp[0]=0，其余设为无穷作为虚拟边界", "逐行从左到右更新", "返回最后一列"],
    "每条到当前格的路径最后一步必来自上或左；两来源的最优子结构成立，取小并加固定格值即当前最优。",
    "左上角需要能从虚拟 dp[0] 正确进入，本实现每行对 col=0 单独处理；遍历方向不能反。",
    "二维最值路径、编辑距离和 LCS 都可以在明确旧行/新行依赖后压缩空间。",
r'''// ---------- Solution ----------
class Solution {
public:
    int minPathSum(vector<vector<int>>& grid) {
        const int cols = static_cast<int>(grid[0].size());
        vector<int> dp(cols, INT_MAX);
        dp[0] = 0;
        for (const auto& row : grid) {
            for (int col = 0; col < cols; ++col) {
                if (col == 0) dp[col] = dp[col] + row[col];
                else dp[col] = min(dp[col], dp[col - 1]) + row[col];
            }
        }
        return dp[cols - 1];
    }
};''')


add(300, "耐心排序 tails + 二分",
    "tails[len-1] 保存长度为 len 的递增子序列所能拥有的最小结尾；结尾越小，未来越容易继续延长。",
    ["遍历每个 x", "lower_bound 找第一个 >=x 的结尾并替换", "若不存在则追加，tails 长度即答案"],
    "替换不会改变已存在的子序列长度，只会让该长度结尾更优；追加说明 x 大于所有结尾，能把最长序列延长一位。",
    "严格递增必须用 lower_bound；tails 本身不一定是一条真实 LIS，只保证长度与最优结尾；若需还原序列要记录前驱。",
    "“为每个长度保留最优结尾”是状态支配压缩，可推广到俄罗斯套娃、最长链和二维偏序。",
r'''// ---------- Solution ----------
class Solution {
public:
    int lengthOfLIS(vector<int>& nums) {
        vector<int> tails;
        for (int value : nums) {
            auto position = lower_bound(tails.begin(), tails.end(), value);
            if (position == tails.end()) tails.push_back(value);
            else *position = value;
        }
        return static_cast<int>(tails.size());
    }
};''')


add(1143, "二维序列 DP + 一维滚动",
    "dp[j] 表示当前 text1 前缀与 text2 前 j 个字符的 LCS；字符相等取左上角+1，否则取上方与左方最大。",
    ["dp 初始化为 0", "逐个扫描 text1 字符，diagonal 保存更新前 dp[j-1]", "相等写 diagonal+1，不等写 max(dp[j],dp[j-1])"],
    "LCS 最后字符若匹配，可由去掉两字符的最优解加一；若不匹配，至少舍弃其中一侧末字符，两个子问题覆盖全部可能。",
    "一维压缩必须先保存旧 dp[j] 再更新 diagonal；dp[j-1] 已是当前行，dp[j] 尚是上一行。",
    "编辑距离、最长公共子串和序列对齐都从两个前缀状态推导，关键是分清左、上、左上含义。",
r'''// ---------- Solution ----------
class Solution {
public:
    int longestCommonSubsequence(string text1, string text2) {
        vector<int> dp(text2.size() + 1, 0);
        for (char a : text1) {
            int diagonal = 0;
            for (int j = 1; j <= static_cast<int>(text2.size()); ++j) {
                int oldAbove = dp[j];
                if (a == text2[j - 1]) dp[j] = diagonal + 1;
                else dp[j] = max(dp[j], dp[j - 1]);
                diagonal = oldAbove;
            }
        }
        return dp.back();
    }
};''')


add(416, "0/1 背包：是否能达到半和",
    "总和为奇数不可能等分；否则问题变成从每个元素最多选一次，能否凑出 target=sum/2。",
    ["计算总和并排除奇数", "dp[0]=true", "对每个 value 从 target 向下更新 dp[s]|=dp[s-value]"],
    "倒序保证转移只读取加入当前元素之前的状态，所以每项至多使用一次；dp[target] 真当且仅当存在目标子集，其补集和也相等。",
    "容量必须倒序，否则会变成完全背包重复使用元素；可在 dp[target] 为真时提前返回。",
    "子集和、等分集合和资源选择都是 0/1 背包；布尔、计数、最大价值只改变 dp 的值域和聚合操作。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        const int total = accumulate(nums.begin(), nums.end(), 0);
        if (total % 2 != 0) return false;
        const int target = total / 2;
        vector<char> reachable(target + 1, false);
        reachable[0] = true;
        for (int value : nums) {
            for (int sum = target; sum >= value; --sum) {
                reachable[sum] = reachable[sum] || reachable[sum - value];
            }
        }
        return reachable[target];
    }
};''')


add(494, "符号选择转 0/1 背包计数",
    "设正号元素和为 P、负号元素和为 N，则 P-N=target 且 P+N=total，故 P=(total+target)/2。",
    ["检查 |target|<=total 且 total+target 为偶数", "ways[0]=1", "每个数对容量从 targetSum 向下做 ways[s]+=ways[s-value]"],
    "代数变换在符号方案与和为 P 的下标子集间建立一一对应；倒序计数确保每个下标只选择一次，包含 0 时方案数会正确翻倍。",
    "不能只判断 total<target，负 target 也要看绝对值；奇偶不符直接为 0；0 是不同符号选择，计数不可去重。",
    "把正负、两组分配问题化为子集和，是背包建模的重要技巧；先列方程往往比直接设计 DP 更清晰。",
r'''// ---------- Solution ----------
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        const int total = accumulate(nums.begin(), nums.end(), 0);
        if (abs(target) > total || (total + target) % 2 != 0) return 0;
        const int positiveSum = (total + target) / 2;
        vector<long long> ways(positiveSum + 1, 0);
        ways[0] = 1;
        for (int value : nums) {
            for (int sum = positiveSum; sum >= value; --sum) {
                ways[sum] += ways[sum - value];
            }
        }
        return static_cast<int>(ways[positiveSum]);
    }
};''')


add(322, "完全背包：最少硬币数",
    "dp[a] 表示组成金额 a 的最少硬币数；每个状态枚举最后使用的硬币，来源为 dp[a-coin]+1。",
    ["dp[0]=0，其余设为 amount+1", "金额从 1 到 amount，枚举 coin<=a", "取最小转移，哨兵未改变则返回 -1"],
    "任意最优方案取出最后一枚 coin 后，剩余必是金额 a-coin 的最优方案，否则可替换得更优；枚举所有 coin 覆盖全部可能。",
    "硬币可无限使用，所以状态可从较小金额反复转移；无解必须返回 -1；amount=0 返回 0。",
    "完全背包的遍历顺序允许当前物品重复使用；求方案数时还要区分组合顺序与排列顺序。",
r'''// ---------- Solution ----------
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        vector<int> dp(amount + 1, amount + 1);
        dp[0] = 0;
        for (int current = 1; current <= amount; ++current) {
            for (int coin : coins) {
                if (coin <= current) dp[current] = min(dp[current], dp[current - coin] + 1);
            }
        }
        return dp[amount] == amount + 1 ? -1 : dp[amount];
    }
};''')


add(312, "区间 DP：枚举区间内最后戳破的气球",
    "给两端补值 1；dp[left][right] 表示开区间 (left,right) 内全部戳完的最大收益，枚举最后被戳的 k，此时它的邻居确定为 left/right。",
    ["构造带边界 1 的 values", "按开区间长度从小到大", "枚举 k，转移 dp[left][k]+values[left]*values[k]*values[right]+dp[k][right]"],
    "虽然第一枚气球邻居难确定，但最后一枚的邻居必是区间边界；所有戳破顺序按其最后元素唯一分类，左右子区间彼此独立。",
    "状态是开区间，长度至少 2 才有内部元素；遍历区间长度必须保证子区间已计算；不要贪心当前最大收益。",
    "区间 DP 常通过枚举“最后合并/最后操作/分割点”解除动态邻接，例如矩阵链乘、石子合并和多边形三角剖分。",
r'''// ---------- Solution ----------
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        vector<int> values;
        values.reserve(nums.size() + 2);
        values.push_back(1);
        values.insert(values.end(), nums.begin(), nums.end());
        values.push_back(1);
        const int n = static_cast<int>(values.size());
        vector<vector<int>> dp(n, vector<int>(n, 0));

        for (int width = 2; width < n; ++width) {
            for (int left = 0; left + width < n; ++left) {
                int right = left + width;
                for (int last = left + 1; last < right; ++last) {
                    dp[left][right] = max(dp[left][right],
                        dp[left][last] + values[left] * values[last] * values[right]
                        + dp[last][right]);
                }
            }
        }
        return dp[0][n - 1];
    }
};''')


add(210, "Kahn 拓扑排序：同时输出可行顺序",
    "与课程可完成性相同，但把每个出队的入度零课程追加到 order；最终长度不足说明存在环。",
    ["建立 b->a 邻接与入度", "所有入度 0 课程入队", "逐个输出并释放后继；结束后检查 order.size()"],
    "出队节点在当时没有未完成先修，故输出前缀始终合法；DAG 总能继续找到入度零点，环则会让剩余节点永远无法入队。",
    "先修边方向很容易写反；答案不唯一，本地判题应按拓扑合法性而不是固定序列比较；有环返回空数组。",
    "任何依赖调度都可把拓扑排序结果作为执行计划，并用未输出节点定位环形依赖。",
r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        vector<vector<int>> next(numCourses);
        vector<int> indegree(numCourses, 0);
        for (const auto& relation : prerequisites) {
            next[relation[1]].push_back(relation[0]);
            ++indegree[relation[0]];
        }
        queue<int> ready;
        for (int course = 0; course < numCourses; ++course)
            if (indegree[course] == 0) ready.push(course);

        vector<int> order;
        while (!ready.empty()) {
            int course = ready.front();
            ready.pop();
            order.push_back(course);
            for (int dependent : next[course])
                if (--indegree[dependent] == 0) ready.push(dependent);
        }
        return static_cast<int>(order.size()) == numCourses ? order : vector<int>{};
    }
};''')


add(79, "网格回溯：路径内标记与恢复",
    "搜索状态由当前位置和 word 下标组成；同一条路径内格子不能复用，可临时改写字符作为 visited，回退后恢复。",
    ["从每个格尝试匹配 word[0]", "字符匹配后若已到末尾则成功，否则标记并搜索四邻", "无论成功失败都恢复当前字符"],
    "递归只沿相邻且未在路径中的匹配字符前进，所以找到的路径必合法；任意合法路径会从其首格开始按方向被枚举。",
    "标记只应在当前递归路径生效，必须恢复；同格不能二次使用；大小写都可能出现，哨兵选输入不会含的字符。",
    "棋盘单词、迷宫简单路径和拼图搜索都需要区分全局 visited 与仅当前路径 visited。",
r'''// ---------- Solution ----------
class Solution {
    int rows = 0, cols = 0;

    bool search(vector<vector<char>>& board, const string& word, int r, int c, int index) {
        if (r < 0 || r >= rows || c < 0 || c >= cols || board[r][c] != word[index]) {
            return false;
        }
        if (index + 1 == static_cast<int>(word.size())) return true;

        const char saved = board[r][c];
        board[r][c] = '\0';
        bool found = search(board, word, r + 1, c, index + 1)
                  || search(board, word, r - 1, c, index + 1)
                  || search(board, word, r, c + 1, index + 1)
                  || search(board, word, r, c - 1, index + 1);
        board[r][c] = saved;  // 即使 found=true 也恢复，保持输入不变
        return found;
    }

public:
    bool exist(vector<vector<char>>& board, string word) {
        rows = static_cast<int>(board.size());
        cols = static_cast<int>(board[0].size());
        for (int r = 0; r < rows; ++r)
            for (int c = 0; c < cols; ++c)
                if (search(board, word, r, c, 0)) return true;
        return false;
    }
};''')


add(139, "前缀可达 DP",
    "dp[i] 表示前 i 个字符能否被字典词完整切分；从每个可达位置 i 尝试接一个字典词，把终点标为可达。",
    ["字典放入哈希集合并求最大词长", "dp[0]=true", "对可达 i 枚举长度并检查 s.substr(i,len)，命中则置 dp[i+len]"],
    "任一合法切分的最后一个词从某个可达前缀开始，转移会发现它；反之每次转移都在合法前缀后追加字典词，因此新状态也合法。",
    "同一字典词允许重复使用；空前缀必须设为 true；最大词长可限制无意义枚举。",
    "字符串分段、解码和路径可达 DP 的核心都是“前缀状态 + 最后一段/下一段选择”。",
r'''// ---------- Solution ----------
class Solution {
public:
    bool wordBreak(string s, vector<string>& wordDict) {
        unordered_set<string> words(wordDict.begin(), wordDict.end());
        int maxLength = 0;
        for (const string& word : wordDict) maxLength = max(maxLength, static_cast<int>(word.size()));
        vector<char> reachable(s.size() + 1, false);
        reachable[0] = true;

        for (int start = 0; start < static_cast<int>(s.size()); ++start) {
            if (!reachable[start]) continue;
            for (int length = 1; length <= maxLength && start + length <= static_cast<int>(s.size()); ++length) {
                if (words.count(s.substr(start, length))) reachable[start + length] = true;
            }
        }
        return reachable[s.size()];
    }
};''')


add(152, "双状态 DP：同时保留最大与最小乘积后缀",
    "负数会交换最大和最小的角色，因此对每个位置必须同时保存“以此结尾”的最大、最小乘积。",
    ["用首元素初始化 currentMax/currentMin/best", "遇负数先交换最大最小", "分别在单独从 x 开始与延续旧后缀间取 max/min，再更新 best"],
    "所有以当前位置结尾的子数组只有 x、旧最大*x、旧最小*x 三类候选；负数交换后按 max/min 更新恰覆盖三者。",
    "不能只维护最大值；0 会自然让状态从 0 或后续元素重启；全负或单元素时 best 不能初始化为 0。",
    "当运算不保持单调性时要保留多个极值状态，例如带负权乘积、绝对值和符号状态 DP。",
r'''// ---------- Solution ----------
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int currentMax = nums[0];
        int currentMin = nums[0];
        int best = nums[0];
        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            const int value = nums[i];
            if (value < 0) swap(currentMax, currentMin);
            currentMax = max(value, currentMax * value);
            currentMin = min(value, currentMin * value);
            best = max(best, currentMax);
        }
        return best;
    }
};''')
