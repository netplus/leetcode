"""Reviewed C++ key-code comments for Week 1 / Day 6.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK1_DAY6 = {
    252: r'''// ---------- Solution ----------
class Solution {
public:
    bool canAttendMeetings(vector<vector<int>>& intervals) {
        // 按 start 排序后，时间线具有单调顺序：若某场会议与紧邻前驱都不重叠，
        // 就无需再回头和更早会议逐一比较。排序把原来的任意两两检查压缩成相邻边界检查。
        sort(intervals.begin(), intervals.end());

        for (int i = 1; i < static_cast<int>(intervals.size()); ++i) {
            // 本仓库把会议区间解释为半开区间 [start,end)：
            // 当前 start == 前一场 end 时，前一场已经恰好结束，不算重叠；
            // 只有 current.start < previous.end 才说明两场时间真正交叠。
            if (intervals[i][0] < intervals[i - 1][1]) return false;
        }

        // 所有相邻边界都不冲突，则整条按开始时间排序的时间线无冲突。
        return true;
    }
};''',

    56: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        // 按起点排序后，未来区间的 start 只会越来越大。
        // 这消除了“后面还有更靠左区间回来接上旧结果”的可能性，
        // 因而可以从左到右维护唯一一段尚未封口的活动并集。
        sort(intervals.begin(), intervals.end());

        vector<vector<int>> answer;
        for (const auto& interval : intervals) {
            // answer.back() 不是简单的“原数组前一个区间”，
            // 而是所有已扫描、且仍与当前连通块相接的区间合并后的活动并集。
            if (answer.empty() || interval[0] > answer.back()[1]) {
                // 题目是闭区间：只有 start > 当前并集 end 才出现真正空隙；
                // start == end 仍共享端点，必须归到同一个并集。
                // 一旦出现空隙，排序保证以后所有 start 更大，旧 answer.back()
                // 永远不可能再被未来区间连接，因此它已经永久封口，当前区间开启新段。
                answer.push_back(interval);
            } else {
                // 当前区间与活动并集有重叠/接触，左端无需变化；
                // 只需把活动并集的右端扩到两者更远的那个 end。
                // 若当前 interval 完全被包含，max 会保持原 end 不变。
                answer.back()[1] = max(answer.back()[1], interval[1]);
            }
        }

        return answer;
    }
};''',
}
