// ============================================================================
// LC-84：柱状图中最大的矩形
// 难度：困难
// 优先级：P1（进阶）
// 学习进度：第 2 周 / 第 12 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定 n 个非负整数，用来表示柱状图中各个柱子的高度。
// 每个柱子彼此相邻，且宽度为 1。
// 求在该柱状图中，能够勾勒出来的矩形的最大面积。
//
// 约束与要求：
//   - 1 <= heights.length <=10^5
//   - 0 <= heights[i] <= 10^4
//
// 复杂度目标：O(n) 时间（单调栈）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜单调递增栈：高度下降时结算矩形
// - 核心要点：
//   1. 思路起点：栈保存高度单调不降的柱下标；遇到更矮柱时，被弹柱的右边界确定，弹栈后的新栈顶确定其左边界。
//   2. 执行逻辑：1. 扫描到 n，并在末尾使用高度 0 哨兵；2. 当前高度更低时反复弹出并计算宽度；3. 压入当前下标。
//   3. 为什么这样做：柱 h 被弹时，栈顶之后到 i-1 的柱高都至少为 h，且两侧是首个更矮位置，所以此时得到以 h 为最低柱的最大矩形。
// - 边界与易错点：宽度是 i-leftBoundary-1；末尾哨兵用于结算所有残留柱；栈存下标而不是只存高度。
// - 举一反三：“找左右首个更小元素”可用于最大矩形、子数组最小值贡献和笛卡尔树构建。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：柱状图中的最大矩形面积。
// test.in 的预期输出：10
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int largestRectangleArea(vector<int>& heights) {
        stack<int> increasing;
        long long best = 0;
        const int n = static_cast<int>(heights.size());
        for (int i = 0; i <= n; ++i) {
            const int currentHeight = (i == n) ? 0 : heights[i];
            while (!increasing.empty() && heights[increasing.top()] > currentHeight) {
                const int height = heights[increasing.top()];
                increasing.pop();
                const int leftBoundary = increasing.empty() ? -1 : increasing.top();
                best = max(best, 1LL * height * (i - leftBoundary - 1));
            }
            increasing.push(i);
        }
        return static_cast<int>(best);
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
    cout << sol.largestRectangleArea(a) << "\n";
    return 0;
}

