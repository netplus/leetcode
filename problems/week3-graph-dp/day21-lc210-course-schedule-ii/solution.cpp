// ============================================================================
// LC-210：课程表 II
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 21 天
// ----------------------------------------------------------------------------
// 题目描述：
// 现在你总共有 numCourses 门课需要选，记为 0 到 numCourses - 1。
// 给你一个数组 prerequisites，其中 prerequisites[i] = [a_i, b_i]，表示在选修课程 a_i 前必须先选修 b_i。
// 例如，想要学习课程 0，你需要先完成课程 1，我们用一个匹配来表示：[0,1]。
// 返回你为了学完所有课程所安排的学习顺序。
// 可能会有多个正确的顺序，你只要返回任意一种就可以了。
// 如果不可能完成所有课程，返回一个空数组。
//
// 题目示例：
// 示例 1：
//   Input: numCourses = 2, prerequisites = [[1,0]]
//   Output: [0,1]
//   Explanation: There are a total of 2 courses to take. To take course 1 you should have finished course 0. So the correct course order is [0,1].
//
// 示例 2：
//   Input: numCourses = 4, prerequisites = [[1,0],[2,0],[3,1],[3,2]]
//   Output: [0,2,1,3]
//   Explanation: There are a total of 4 courses to take. To take course 3 you should have finished both courses 1 and 2. Both courses 1 and 2 should be taken after you finished course 0.
//   So one correct course order is [0,1,2,3]. Another correct ordering is [0,2,1,3].
//
// 示例 3：
//   Input: numCourses = 1, prerequisites = []
//   Output: [0]
//
// 约束与要求：
//   - 1 <= numCourses <= 2000
//   - 0 <= prerequisites.length <= numCourses * (numCourses - 1)
//   - prerequisites[i].length == 2
//   - 0 <= a_i, b_i < numCourses
//   - a_i != b_i
//   - 所有 [a_i, b_i] 互不相同
//
// 复杂度目标：O(V+E) 时间（拓扑排序）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜LC-207 的输出型迁移：Kahn 拓扑排序把可完成性证明直接变成课程顺序
//
// 1. 图像直觉
//   prerequisites = [[1,0],[2,0],[3,1],[3,2]]
//
//   依赖图：
//   0 -> 1 -> 3
//    \-> 2 -/
//
//   初始 indegree：
//   0:0  1:1  2:1  3:2
//   ready=[0]
//
//   输出 0 后，删除它的两条出边：
//   1、2 的入度都变 0，所以两者都可以进入 ready。
//   此时输出 1 还是 2 都合法，这正解释了为什么答案不唯一。
//
//   若先输出 2，再输出 1：
//   order = [0,2,1,3]
//
//   每个被写入 order 的节点，在写入那一刻都已经没有未完成先修。
//
// 2. 一句话核心
//   原样执行 LC-207 的入度消除，只是在每个零入度课程出队时把它记录下来；若最终记录了全部课程，这个记录顺序就是答案。
//
// 3. 公式 / 不变量
//   对 prerequisite [a,b]：
//   edge: b -> a
//   indegree[a]++
//
//   ready = 所有 indegree[v]==0 的课程。
//
//   每次取出 course：
//   order.push_back(course)
//   对每个 dependent in next[course]：
//       indegree[dependent]--
//       若变成 0：加入 ready
//
//   循环不变量：
//   order 中每门课出现时，它的所有先修都已经出现在 order 更早的位置；ready 中所有节点当前都没有尚未输出的前驱。
//
//   结束条件：
//   order.size()==numCourses -> order 是一个拓扑序
//   否则 -> 剩余子图存在有向环，返回空数组。
//
// 4. 执行步骤
//   1. 按照 [a,b] 表示 b 是 a 的先修，建立 b->a 邻接表并累计 indegree[a]；不要把方向写反
//   2. 把所有初始入度为 0 的课程放入 ready，它们不依赖任何尚未完成课程，可以作为拓扑序的起点
//   3. 循环弹出一个 ready 课程，立即追加到 order；这个动作就是 LC-207 中“处理一个可完成课程”的可见输出
//   4. 遍历它的所有后继并递减入度；某后继恰好降到 0 时，说明它最后一个未完成先修刚被解除，将其加入 ready
//   5. 队列清空后比较 order.size() 与 numCourses；相等返回 order，否则返回空 vector 表示存在环
//   记忆：LC-207 会不会做完；LC-210 把“做的顺序”顺手记下来。
//
// 5. 为什么不会漏 / 不会重
//   一个课程只有在入度降为 0 后才会进入 ready，因此它被输出时，所有指向它的先修边都已经由更早输出的前驱删除，故 order 的每个前缀都满足先修约束。每条边只在其起点出队时处理一次，每个节点只会在入度第一次降到 0 时入队一次，不会重复。若图是 DAG，任意非空剩余 DAG 必存在零入度点，所以过程能输出全部节点；若最终仍有节点未输出，剩余子图没有零入度点，只可能包含有向环，因此不存在完成全部课程的合法顺序。
//
// 6. 边界与易错点
//   最常见错误仍是把 [a,b] 建成 a->b。合法拓扑序可能有很多种，不应把算法绑死到某个官方输出；本地/外部验证应检查先修相对顺序。numCourses=1 且 prerequisites 为空时应返回 [0]；存在环时返回真正的空 vector，而本地 main 才把它打印成 -1。
//
// 7. 举一反三
//   这是 Day 15 LC-207 的零新模板迁移：同一个 Kahn 过程，LC-207 的输出是 bool，LC-210 的输出是过程轨迹 order。构建系统、任务调度、依赖安装、编译顺序都可直接输出这条拓扑执行计划；若需要字典序最小顺序，只需把 ready 的 FIFO queue 换成小根堆，正确性不变。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：numCourses m。
//   接下来 m 行：a b (a 之前必须先处理 b)。
//   输出：一种以空格分隔的课程顺序；无法完成时输出 -1。
// test.in 的预期输出：0 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> findOrder(int numCourses, vector<vector<int>>& prerequisites) {
        // [a,b] 表示“学 a 前必须先学 b”，所以图边必须是 b -> a；
        // indegree[a] 统计 a 当前还剩多少个尚未完成的直接先修课程。
        vector<vector<int>> next(numCourses);
        vector<int> indegree(numCourses, 0);
        for (const auto& relation : prerequisites) {
            next[relation[1]].push_back(relation[0]);
            ++indegree[relation[0]];
        }

        // 初始入度为 0 的课程没有任何未完成先修，可以立即作为拓扑序的候选起点。
        queue<int> ready;
        for (int course = 0; course < numCourses; ++course)
            if (indegree[course] == 0) ready.push(course);

        vector<int> order;
        while (!ready.empty()) {
            int course = ready.front();
            ready.pop();

            // 一个课程只有在入度已为 0 时才会进入 ready，因此此刻把它写入 order 一定满足全部先修约束。
            order.push_back(course);

            // “完成 course”相当于删除它的所有出边；某后继只有在最后一条未完成先修边被删除、
            // 入度恰好降到 0 时才第一次入队，因此每门课不会重复进入 ready。
            for (int dependent : next[course])
                if (--indegree[dependent] == 0) ready.push(dependent);
        }

        // 若输出了全部课程，order 就是一条合法拓扑序；若数量不足，剩余节点始终无法产生零入度点，说明被有向环卡住。
        return static_cast<int>(order.size()) == numCourses ? order : vector<int>{};
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
    auto ans = sol.findOrder(nc, pre);
    if (ans.empty()) cout << -1 << "\n";
    else for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
