// ============================================================================
// LC-739：每日温度
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 12 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个整数数组 temperatures，表示每天的温度，返回一个数组 answer，其中 answer[i] 是指对于第 i 天，下一个更高温度出现在几天后。
// 如果气温在这之后都不会升高，请在该位置用 0 来代替。
//
// 约束与要求：
//   - 1 <= temperatures.length <= 10^5
//   - 30 <= temperatures[i] <= 100
//
// 复杂度目标：O(n) 时间（单调栈）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜单调递减栈：等待下一个更大值
// - 核心要点：
//   1. 思路起点：栈保存尚未找到更暖日期的下标，且对应温度单调不增；当前更高温度可一次结算栈顶若干日期。
//   2. 执行逻辑：1. 从左到右扫描日期；2. 当前温度更高时反复弹栈并填写距离；3. 把当前下标压栈等待未来结算。
//   3. 为什么这样做：被弹下标之间没有更早的更高温度，否则早已被弹；当前日是它遇到的第一个严格更暖日。未弹出的日期最终答案保持 0。
// - 边界与易错点：题目要求严格更高，相等不能弹；栈存下标才能计算天数；每个下标最多进出一次。
// - 举一反三：下一个更大/更小元素、股票跨度和柱状图边界都通过单调栈批量结算待定元素。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：n 个以空格分隔的等待天数。
// test.in 的预期输出：1 1 4 2 1 1 0 0
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> dailyTemperatures(vector<int>& temperatures) {
        vector<int> answer(temperatures.size(), 0);
        stack<int> pending;
        for (int day = 0; day < static_cast<int>(temperatures.size()); ++day) {
            while (!pending.empty() && temperatures[day] > temperatures[pending.top()]) {
                const int previous = pending.top();
                pending.pop();
                answer[previous] = day - previous;
            }
            pending.push(day);
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n;
    if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.dailyTemperatures(a);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

