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

    57: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> answer;
        int i = 0, n = static_cast<int>(intervals.size());

        // 第一阶段：只复制“完全位于 newInterval 左侧”的区间。
        // 闭区间下必须满足 old.end < new.start 才真正分离；若 end==start 仍共享端点，应进入合并阶段。
        while (i < n && intervals[i][1] < newInterval[0]) {
            answer.push_back(intervals[i++]);
        }

        // 第二阶段：连续吸收所有与当前 newInterval 相交的区间。
        // 注意判断使用的是“不断扩张后的” newInterval[1]：
        // 前一个区间把右端撑大后，原本不与初始新区间相交的后续区间也可能被新右端触及。
        while (i < n && intervals[i][0] <= newInterval[1]) {
            // 相交后，新活动并集的边界就是两者端点的 min/max。
            // 这里直接修改 newInterval，使它承担 LC-56 中“当前活动并集”的角色。
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            ++i;
        }

        // 中段结束有两种可能：已扫描完，或下一段 start > 当前 new.end。
        // 后一种情况下，因原 intervals 已按 start 排序，后续区间都永久在右侧，
        // 所以当前 newInterval 已经最终封口，可以只输出一次。
        answer.push_back(newInterval);

        // 第三阶段：剩余区间全在最终 newInterval 右侧；原输入又已排序且互不重叠，
        // 因此无需再做任何合并或排序，按原顺序直接追加即可。
        while (i < n) answer.push_back(intervals[i++]);

        return answer;
    }
};''',

    179: r'''// ---------- Solution ----------
class Solution {
public:
    string largestNumber(vector<int>& nums) {
        // 最终答案可能远超整数范围；先转成十进制字符串，后续只讨论“拼接顺序”。
        vector<string> parts;
        parts.reserve(nums.size());
        for (int value : nums) parts.push_back(to_string(value));

        sort(parts.begin(), parts.end(), [](const string& a, const string& b) {
            // 固定其它前后文时，a/b 只有 ab 与 ba 两种局部顺序。
            // 若 a+b 更大，就必须让 a 在 b 前，否则交换二者会让最终拼接结果变大。
            // 使用严格 > 很重要：当 a+b == b+a 时返回 false，满足 std::sort 的严格比较要求。
            return a + b > b + a;
        });

        // 排序后的 parts[0] 已经是最应该放在最前面的元素。
        // 若它仍为 "0"，说明没有任何非零字符串能排到 0 前面，因此所有输入都只能是 0；
        // 规范化返回单个 "0"，而不是 "000..."。
        if (parts[0] == "0") return "0";

        string answer;
        // 比较器已经确定了全局相对顺序；这里按该顺序直接拼接即可。
        for (const string& part : parts) answer += part;
        return answer;
    }
};''',
}
