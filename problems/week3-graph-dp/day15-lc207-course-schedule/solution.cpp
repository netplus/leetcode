// ============================================================================
// LC-207：课程表
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 15 天
// ----------------------------------------------------------------------------
// 题目描述：
// 你这个学期必须选修 numCourses 门课程，记为 0 到 numCourses - 1。
// 在选修某些课程之前需要一些先修课程。
// 先修课程按数组 prerequisites 给出，其中 prerequisites[i] = [a_i, b_i]，表示如果要学习课程 a_i 则必须先学习课程 b_i _。
// 例如，先修课程对 [0, 1] 表示：想要学习课程 0，你需要先完成课程 1。
// 请你判断是否可能完成所有课程的学习？如果可以，返回 true；否则，返回 false。
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
// 解法精讲｜Kahn 拓扑排序：逐步删除入度为零节点
// - 核心要点：
//   1. 思路起点：先修关系 b->a 构成有向图；若能反复选出当前无依赖课程并删除其出边，最终处理全部节点则无环。
//   2. 执行逻辑：1. 建立邻接表和每门课入度；2. 所有入度 0 课程入队；3. 出队计数并降低后继入度，新变 0 时入队。
//   3. 为什么这样做：入队课程的所有先修均已处理，所以选择合法；有向无环图必至少有一个入度 0 节点，若处理数不足则剩余子图必含环。
// - 边界与易错点：边方向应从先修课指向后修课；重复入队通过仅在入度降到 0 时加入来避免；孤立课程也应计数。
// - 举一反三：构建顺序、任务依赖、软件包安装和流水线调度都可以拓扑排序检测环并产生执行序列。
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

