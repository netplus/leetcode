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
}
