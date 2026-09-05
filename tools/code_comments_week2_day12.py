"""Reviewed C++ key-code comments for Week 2 / Day 12.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK2_DAY12 = {
    20: r'''// ---------- Solution ----------
class Solution {
public:
    bool isValid(string s) {
        // openings 保存“已经开启、但尚未闭合”的左括号；
        // 栈顶就是最近开启的结构，也是下一只右括号唯一允许关闭的对象。
        stack<char> openings;

        for (char c : s) {
            if (c == '(' || c == '[' || c == '{') {
                // 左括号开启新的嵌套上下文，先欠下一笔等待未来关闭的账。
                openings.push(c);
                continue;
            }

            // 当前字符既然是右括号，就必须关闭一个已有上下文；
            // 若栈已空，说明没有任何左括号可与它配对，立即非法。
            if (openings.empty()) return false;

            // 把当前右括号映射成“它唯一允许匹配的左括号类型”。
            const char expected = (c == ')') ? '(' : (c == ']') ? '[' : '{';

            // 合法嵌套要求最近开启的结构最先关闭；
            // 即使栈更深处存在同类型左括号，也不能越过错误的栈顶去匹配它。
            if (openings.top() != expected) return false;
            openings.pop();
        }

        // 扫描结束后仍有左括号留栈，说明这些上下文从未被关闭，同样非法。
        return openings.empty();
    }
};''',

    155: r'''// ---------- Solution ----------
class Solution {
    // 每一层同时保存“实际值”和“压入这一层之后整个栈的最小值”。
    // 因此栈顶 pair 自带当前状态摘要，getMin 无需回头扫描历史元素。
    stack<pair<int, int>> values;

public:
    void push(int val) {
        // 新状态的最小值只依赖“上一层最小值”和当前 val；空栈时 val 自己就是最小值。
        const int minimum = values.empty() ? val : min(val, values.top().second);
        values.push({val, minimum});
    }

    // pair 整层弹出后，下面一层原先保存的 minimum 自动恢复成当前栈的正确最小值；无需重算。
    void pop() { values.pop(); }

    // first 是当前真实栈顶值；second 是当前整个栈的最小摘要。
    int top() { return values.top().first; }
    int getMin() { return values.top().second; }
};''',

    739: r'''// ---------- Solution ----------
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        // 默认 0 正好代表“直到末尾都没有更暖日期”；只有被未来事件结算的下标才会改写。
        vector<int> answer(temperatures.size(), 0);

        // pending 存“还没找到第一个更暖日期”的历史下标。
        // 下标从底到顶递增，对应温度单调不增；栈顶是最近、最先有机会被当前温度结算的日期。
        stack<int> pending;

        for (int day = 0; day < static_cast<int>(temperatures.size()); ++day) {
            // 当前温度严格高于栈顶时，day 就是该历史日期第一次遇到的更暖日：
            // 若更早已有答案，它早已在当时被弹栈，不可能继续留在 pending。
            // 相等温度不能弹，因为题目要求 strictly warmer。
            while (!pending.empty() && temperatures[day] > temperatures[pending.top()]) {
                const int previous = pending.top();
                pending.pop();

                // pending 存的是下标而非温度，正是为了直接得到等待天数 day-previous。
                answer[previous] = day - previous;
            }

            // 当前日期暂时不知道未来答案，加入未决集合；以后由第一个严格更暖的日期负责结算。
            pending.push(day);
        }

        // 扫描结束仍留在栈中的日期从未遇到更暖日，保持初始化的 0。
        return answer;
    }
};''',

    84: r'''// ---------- Solution ----------
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        // increasing 存尚未确定右侧更矮边界的柱子下标；对应高度从栈底到栈顶单调不降。
        stack<int> increasing;
        long long best = 0;
        const int n = static_cast<int>(heights.size());

        // 多跑一轮 i==n，并把它视作高度 0 的虚拟右哨兵，
        // 用来强制结算那些直到数组末尾都没遇到更矮柱的历史高度。
        for (int i = 0; i <= n; ++i) {
            const int currentHeight = (i == n) ? 0 : heights[i];

            // 当前柱更矮时，被弹柱子的右侧第一个“严格更矮”位置刚刚确定为 i。
            while (!increasing.empty() && heights[increasing.top()] > currentHeight) {
                const int height = heights[increasing.top()];
                increasing.pop();

                // 弹栈后的新栈顶是当前实现计算这根柱宽度时的左阻挡位置；
                // 因为这里使用严格 > 弹栈，相等高度允许共存，所以它可能与 height 相等，
                // 不一定是“左侧第一个严格更矮柱”。若栈空则使用虚拟边界 -1。
                const int leftBoundary = increasing.empty() ? -1 : increasing.top();

                // 两个边界下标本身不计入本次矩形，宽度为 i-leftBoundary-1。
                // 对等高柱而言，较晚那根可能先得到较窄宽度；更早的等高柱随后弹出时会覆盖更宽范围，
                // 因而全局最大面积仍不会遗漏。
                best = max(best, 1LL * height * (i - leftBoundary - 1));
            }

            // 当前下标加入未封口集合。i==n 的虚拟哨兵只在最后压栈，循环随即结束，
            // 因此后续不会用它访问 heights[n]。
            increasing.push(i);
        }

        return static_cast<int>(best);
    }
};''',
}
