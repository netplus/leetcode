// ============================================================================
// LC-253：会议室 II
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 26 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一组会议时间区间 intervals，返回为了安排所有会议至少需要的会议室数量。
// 本地适配器使用半开区间 [start, end)：一场会议在另一场开始时结束，可以复用同一间会议室。
//
// 约束与要求：
//   - 0 <= intervals.length <= 10^4
//   - intervals[i].length == 2
//   - 0 <= start < end <= 10^6
//
// 复杂度目标：O(m log m) 时间（sweep line / heap）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜按开始时间扫描 + 最小结束时间堆
// - 核心要点：
//   1. 思路起点：堆保存当前占用会议室的结束时间；下一会议开始前，所有已结束会议室都可释放，然后为当前会议占用一间。
//   2. 执行逻辑：1. 按开始时间排序；2. 弹出所有 end<=currentStart 的结束时间；3. 压入当前 end，并记录堆大小最大值。
//   3. 为什么这样做：处理当前会议后，堆恰表示与它时间上重叠、仍占用房间的会议；同一时刻的堆大小是所需房间数，对全程取最大即最小容量。
// - 边界与易错点：结束等于开始时房间可复用，所以弹出条件是 <=；应弹出所有已结束项；空输入返回 0。
// - 举一反三：资源复用问题常按开始事件排序，并用最小堆管理最早释放的资源，例如机器调度和连接池容量。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m (区间数量)。
//   接下来 m 行：start end。
//   输出：最少会议室数量。
// test.in 的预期输出：2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        priority_queue<int, vector<int>, greater<int>> endTimes;
        int best = 0;
        for (const auto& meeting : intervals) {
            while (!endTimes.empty() && endTimes.top() <= meeting[0]) endTimes.pop();
            endTimes.push(meeting[1]);
            best = max(best, static_cast<int>(endTimes.size()));
        }
        return best;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m; if (!(cin >> m)) return 0;
    vector<vector<int>> iv(m, vector<int>(2));
    for (int i = 0; i < m; ++i) cin >> iv[i][0] >> iv[i][1];
    cout << sol.minMeetingRooms(iv) << "\n";
    return 0;
}

