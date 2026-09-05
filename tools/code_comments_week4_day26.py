"""Reviewed C++ key-code comments for Week 4 / Day 26.

Entries are added only after an independent per-problem review.
"""

CODE_COMMENTS_WEEK4_DAY26 = {
    42: r'''// ---------- Solution ----------
class Solution {
public:
    int trap(vector<int>& height) {
        int left = 0, right = static_cast<int>(height.size()) - 1;
        // leftMax/rightMax 分别保存已经从两端扫描到当前位置附近时见过的最高边界；
        // 尚未移出 [left,right] 的位置还没有结算最终水量。
        int leftMax = 0, rightMax = 0;
        long long water = 0;

        while (left <= right) {
            // 当 leftMax<=rightMax 时，右侧已经存在高度至少为 rightMax 的真实边界，
            // 因而左端位置的最终水位瓶颈只可能是 leftMax，可以立即结算左端；右侧对称。
            if (leftMax <= rightMax) {
                // 必须先把当前柱纳入左侧最高边界，再做差；这样贡献天然非负，
                // 且 leftMax-height[left] 正是该位置已经确定的最终蓄水量。
                leftMax = max(leftMax, height[left]);
                water += leftMax - height[left];
                ++left;
            } else {
                rightMax = max(rightMax, height[right]);
                water += rightMax - height[right];
                --right;
            }
        }
        return static_cast<int>(water);
    }
};''',

    253: r'''// ---------- Solution ----------
class Solution {
public:
    int minMeetingRooms(vector<vector<int>>& intervals) {
        // 按开始时间推进时间线；之后遇到的会议只会开始得更晚，
        // 所以一场已经结束的旧会议一旦弹出，就永远不需要重新考虑。
        sort(intervals.begin(), intervals.end());

        // endTimes 只保存“已经开始但在当前 start 时刻仍未结束”的会议结束时间；
        // 小根堆让最早释放的房间始终站在堆顶。
        priority_queue<int, vector<int>, greater<int>> endTimes;
        int best = 0;
        for (const auto& meeting : intervals) {
            // 仓库采用半开区间 [start,end)：end==nextStart 时不重叠，可以复用同一房间。
            // 必须弹出所有已结束会议，而不是只弹一个，因为可能同时释放多间房。
            while (!endTimes.empty() && endTimes.top() <= meeting[0]) endTimes.pop();

            // 当前会议从此刻开始占用一间房；push 后堆大小就是这一时刻同时活跃的会议数。
            endTimes.push(meeting[1]);
            best = max(best, static_cast<int>(endTimes.size()));
        }
        // 全程最大并发会议数既是任何方案的下界，也能通过复用最早结束房间实现。
        return best;
    }
};''',

    406: r'''// ---------- Solution ----------
class Solution {
public:
    vector<vector<int>> reconstructQueue(vector<vector<int>>& people) {
        // 先处理高个：当轮到 [h,k] 时，answer 中所有人都满足 height>=h，
        // 因而“前面有多少个会被当前人计数的人”就等于插入位置本身。
        // 同身高必须按 k 升序，让 k 较小者先固定，后续同高者不会插到它前面破坏其计数。
        sort(people.begin(), people.end(), [](const vector<int>& a, const vector<int>& b) {
            if (a[0] != b[0]) return a[0] > b[0];
            return a[1] < b[1];
        });

        vector<vector<int>> answer;
        for (const auto& person : people) {
            // 此刻已有元素全都不矮于 person，所以插入下标 k 后，
            // person 前方恰有 k 个 height>=person[0] 的人；以后加入的更矮者不计入它的约束。
            answer.insert(answer.begin() + person[1], person);
        }
        return answer;
    }
};''',

    621: r'''// ---------- Solution ----------
class Solution {
public:
    int leastInterval(vector<char>& tasks, int n) {
        // 任务只有 A..Z，先统计最终频次；真正撑开冷却时间轴的是最高频任务。
        array<int, 26> frequency{};
        for (char task : tasks) ++frequency[task - 'A'];

        const int maxFrequency = *max_element(frequency.begin(), frequency.end());
        // 可能有多种任务并列最高频，它们都会占据骨架最后一列，因此不能只按一种最高频任务计算。
        const int maxCount = static_cast<int>(count(frequency.begin(), frequency.end(), maxFrequency));

        // 最高频任务有 maxFrequency 次：前 maxFrequency-1 组之间都必须隔出 n 个冷却位置，
        // 所以形成 (maxFrequency-1) 个长度 n+1 的完整块；最后再放 maxCount 个并列最高频任务。
        const int skeleton = (maxFrequency - 1) * (n + 1) + maxCount;

        // skeleton 是冷却约束强制出的下界；tasks.size() 是“每个真实任务至少占一个时间单位”的下界。
        // 其他任务足够多时会把骨架 idle 全填满，答案由任务数主导；否则不可避免的 idle 让骨架主导。
        return max(static_cast<int>(tasks.size()), skeleton);
    }
};''',
}
