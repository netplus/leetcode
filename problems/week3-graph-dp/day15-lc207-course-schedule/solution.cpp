// ============================================================================
// LC-207：课程表
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 15 天
// ----------------------------------------------------------------------------
// 题目描述：
// 你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses - 1。
// 在选修某些课程之前需要一些先修课程。
// 先修课程按数组 prerequisites 给出，其中 prerequisites[i] = [a_i, b_i]，表示如果要学习课程 a_i，则必须先学习课程 b_i。
// 例如，先修课程对 [0, 1] 表示：想要学习课程 0，你需要先完成课程 1。
// 请你判断是否可能完成所有课程的学习？如果可以，返回 true；否则，返回 false。
//
// 题目示例：
// 示例 1：
//   Input: numCourses = 2, prerequisites = [[1,0]]
//   Output: true
//   Explanation: There are a total of 2 courses to take.
//   To take course 1 you should have finished course 0. So it is possible.
//
// 示例 2：
//   Input: numCourses = 2, prerequisites = [[1,0],[0,1]]
//   Output: false
//   Explanation: There are a total of 2 courses to take.
//   To take course 1 you should have finished course 0, and to take course 0 you should also have finished course 1. So it is impossible.
//
// 约束与要求：
//   - 1 <= numCourses <= 2000
//   - 0 <= prerequisites.length <= 5000
//   - prerequisites[i].length == 2
//   - 0 <= a_i, b_i < numCourses
//   - prerequisites[i] 中的所有课程对互不相同
//
// 复杂度目标：O(V+E) 时间（拓扑排序 / DFS）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜拓扑排序：把“先修依赖”变成有向边，反复删除当前入度为 0 的课程
//
// 前置概念
//   对有向图中的边 u -> v：
//   - v 的入度（indegree）是指向 v 的边数量；
//   - 拓扑序（topological order）是一种线性排列，使每条边 u -> v 中的 u 都出现在 v 之前。
//
//   例如 2 -> 1 -> 0 的一个拓扑序就是 [2,1,0]。
//
//   只有无环有向图（DAG）才存在拓扑序。课程表里边表示“先修 -> 后修”，所以入度 0 的课程表示当前已经没有尚未完成的前置依赖，可以作为下一门课。
//
//   不要把“检查环”简化成只寻找一对互相依赖的课程。三门课就能形成更长的环：
//   0 -> 1 -> 2 -> 0。
//   这里不存在任何一对同时具有 a->b 和 b->a，但整体仍然没有合法拓扑序。
//   所以必须验证整个依赖图是否能被拓扑消除完，而不是只做局部成对检查。
//
// 1. 图像直觉
//   例：
//   0 <- 1 <- 2
//
//   边方向按“先修 -> 后修”：
//   2 -> 1 -> 0
//
//   初始入度：
//   indegree[2]=0
//   indegree[1]=1
//   indegree[0]=1
//
//   ready=[2]
//   拿走 2，删除 2->1：1 的入度变 0，于是入队。
//   再拿走 1，0 的入度变 0。
//   最终三门课都能被拿走。
//
//   若是：
//   0 -> 1
//   ^    |
//   |____v
//
//   每个节点入度都 >0，ready 一开始就是空；它们彼此等待，没有任何合法第一步。
//
// 2. 一句话核心
//   把先修关系画成“先修课 -> 后修课”；入度 0 表示当前已经没有未完成前置依赖，持续删除这类节点，能删完全部节点就说明无环。
//
// 3. 公式 / 不变量
//   对 prerequisite [a,b]：
//   edge: b -> a
//   indegree[a] += 1
//
//   ready = {v | indegree[v] == 0}
//
//   删除 course 时，对每条 course -> dependent：
//   indegree[dependent] -= 1
//   若恰好变成 0：dependent 入队
//
//   最终：
//   completed == numCourses <=> 图中不存在有向环。
//
// 4. 执行步骤
//   1. 建立邻接表 next，并按 b->a 的方向统计每门课 indegree；边方向反了会把依赖语义完全颠倒
//   2. 把所有初始 indegree==0 的课程放进 ready，包括没有任何依赖关系的孤立课程
//   3. 每次从 ready 取一门课，completed 加一；这代表它的所有先修都已经被移除，可以安全完成
//   4. 遍历它指向的后修课，把这些课程的剩余入度减一；只有在入度刚降到 0 时才入队一次
//   5. 队列耗尽后检查 completed；若小于 numCourses，剩余子图没有入度 0 节点，因此存在环
//   记忆：[a,b] 读成 b->a；零入度先做，做完就释放后继。
//
// 5. 为什么不会漏 / 不会重
//   任何入度为 0 的节点都没有尚未满足的先修，选择它一定合法。DAG 的任意非空子图都至少存在一个入度为 0 的节点，因此如果图无环，Kahn 过程一定能持续删除直到全部节点处理完。反过来，如果过程提前停下且仍有节点，每个剩余节点都至少有一条来自剩余集合的入边，沿前驱不断回溯必然重复某节点，从而形成有向环。
//
// 6. 边界与易错点
//   最容易错的是边方向：题目给 [a,b]，必须建立 b->a。孤立课程也属于需要完成的节点，所以应计入 completed。只有当入度从 1 降到 0 时入队，避免重复入队。Kahn 会破坏 indegree 数组；若后续还需要原入度，应复制一份。
//
// 7. 举一反三
//   从 LC-200/133 的“遍历可到达节点”升级到“遍历必须遵守依赖顺序”。软件包安装、构建系统、任务 DAG、流水线调度都可用同一模型。后续若题目要求实际课程顺序，只需把每次出队的 course 记录下来，就是拓扑序。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：numCourses m。
//   接下来 m 行：a b (选修 a 之前必须先选修 b)。
//   输出：能完成所有课程时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
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
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int nc, m; if (!(cin >> nc >> m)) return 0;
    vector<vector<int>> pre(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> pre[i][0] >> pre[i][1];
    cout << (sol.canFinish(nc, pre) ? 1 : 0) << "\n";
    return 0;
}
