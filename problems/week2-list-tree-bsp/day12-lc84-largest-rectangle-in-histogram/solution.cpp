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
// 题目示例：
// 示例 1：
//   Input: heights = [2,1,5,6,2,3]
//   Output: 10
//   Explanation: The above is a histogram where width of each bar is 1.
//   The largest rectangle is shown in the red area, which has an area = 10 units.
//
// 示例 2：
//   Input: heights = [2,4]
//   Output: 4
//
// 约束与要求：
//   - 1 <= heights.length <=10^5
//   - 0 <= heights[i] <= 10^4
//
// 复杂度目标：O(n) 时间（单调栈）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜单调递增栈：更矮柱出现时确定矩形的最终左右边界
//
// 0. 优化是怎么来的
//   最直接的做法是把每根柱子当作最低高度，分别向左、向右扩展，直到遇到更矮柱；每根柱都可能重复扫描大片相同区域，最坏 O(n^2)。
//
//   对某根柱 mid 来说，真正需要的只有两个边界：左侧第一个更矮位置和右侧第一个更矮位置。一旦扫描过程中第一次遇到右侧更矮柱，mid 的最大可扩展范围就永久确定，不需要以后再查。
//
//   维护单调不降栈后，当前更矮柱就是被弹元素的 rightLess；弹栈后的新栈顶又恰好暴露出 leftLess，于是一次事件同时得到两个边界并完成面积结算。
//
//   优化类型：重复扩边界 -> 单调栈保存未封口高度，在边界事件到来时一次结算。
//
// 1. 图像直觉
//   对每根柱 h，不要问“它能组成哪些矩形”，而问：
//   “如果把 h 当成矩形最低高度，它最多能向左右延伸到哪里？”
//
//   heights = [2, 1, 5, 6, 2, 3]
//
//   扫描到右侧的 2：
//             [5][6]
//             [5][6]
//             [5][6]
//          1  [5][6] 2
//
//   6 遇到右边第一个更矮的 2 -> 6 的右界确定
//   弹掉 6 后，5 也遇到右边第一个更矮的 2 -> 5 的右界确定
//
//   弹栈后的新栈顶，就是被弹柱左边第一个更矮的位置。
//
// 2. 一句话核心
//   柱子只有在遇到右侧第一个更矮柱时才真正“封口”；此时弹栈后的新栈顶同时给出它左侧第一个更矮柱，于是最大宽度一次确定。
//
// 3. 公式 / 不变量
//   当下标 i 的 currentHeight < heights[top] 时，弹出 mid：
//
//   rightLess = i
//   leftLess  = stack.empty() ? -1 : stack.top()
//   height    = heights[mid]
//   width     = rightLess - leftLess - 1
//   area      = height * width
//
//   此时 [leftLess+1, rightLess-1] 内所有柱高都 >= height，
//   所以这是以 heights[mid] 为最低高度时能取得的最大宽度。
//
// 4. 执行步骤
//   1. 维护柱高单调不降的下标栈；扫描 i=0..n，并把 i==n 看成高度 0 的右哨兵，确保最后残留柱也会被结算
//   2. 若当前高度低于栈顶柱高，就反复弹出 mid；当前 i 是 mid 右侧第一个更矮位置，弹栈后的新栈顶是 mid 左侧第一个更矮位置
//   3. 用 width=i-leftBoundary-1 计算以该柱为最低高度的最大矩形面积；完成所有需要结算的柱后，再把当前下标压栈
//   记忆：LC-739 是“遇到更大值，答案出现”；LC-84 是“遇到更小值，边界封口”。单调栈本质都是等未来事件到来时一次结算历史状态。
//
// 5. 为什么不会漏 / 不会重
//   被弹柱 mid 的右侧第一个更矮位置就是当前 i：此前若出现更矮柱，mid 已经在那时被弹。
//   弹出 mid 后的新栈顶是左侧最近更矮柱；两者之间留下来的柱子都不低于 heights[mid]，因此这一整段都能支撑高度 heights[mid]。
//   再向左或向右扩一格就会碰到更矮柱，故此时得到的宽度已经是“以 mid 为最低柱”的最大宽度。每根柱只压栈一次、弹栈一次，总复杂度 O(n)。
//
// 6. 边界与易错点
//   最容易错的是宽度：两侧更矮柱本身不能包含，因此是 `i-leftBoundary-1`；末尾哨兵用于强制结算递增尾巴；实现使用严格 `>` 弹栈，相等高度可保留，仍能在后续得到正确最大宽度。
//
// 7. 举一反三
//   直接承接 LC-739 的“未决候选 + 未来事件结算”。换个视角，本题是在为每个高度找左右第一个更小元素；这也是子数组最小值贡献、笛卡尔树等问题的核心边界模型。
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
