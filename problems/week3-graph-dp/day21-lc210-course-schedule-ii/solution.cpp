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
// 解法精讲｜Kahn 拓扑排序：同时输出可行顺序
// - 核心要点：
//   1. 思路起点：与课程可完成性相同，但把每个出队的入度零课程追加到 order；最终长度不足说明存在环。
//   2. 执行逻辑：1. 建立 b->a 邻接与入度；2. 所有入度 0 课程入队；3. 逐个输出并释放后继；结束后检查 order.size()。
//   3. 为什么这样做：出队节点在当时没有未完成先修，故输出前缀始终合法；DAG 总能继续找到入度零点，环则会让剩余节点永远无法入队。
// - 边界与易错点：先修边方向很容易写反；答案不唯一，本地判题应按拓扑合法性而不是固定序列比较；有环返回空数组。
// - 举一反三：任何依赖调度都可把拓扑排序结果作为执行计划，并用未输出节点定位环形依赖。
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

