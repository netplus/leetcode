// ============================================================================
// LC-56：合并区间
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 6 天
// ----------------------------------------------------------------------------
// 题目描述：
// 以数组 intervals 表示若干个区间的集合，其中单个区间为 intervals[i] = [start_i, end_i]。
// 请你合并所有重叠的区间，并返回一个不重叠的区间数组，该数组需恰好覆盖输入中的所有区间。
//
// 约束与要求：
//   - 1 <= intervals.length <= 10^4
//   - intervals[i].length == 2
//   - 0 <= start_i <= end_i <= 10^4
//
// 复杂度目标：O(m log m) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜排序后线性合并区间
// - 核心要点：
//   1. 思路起点：按起点排序后，与当前合并段重叠的区间必连续出现，只需比较其起点与当前段终点。
//   2. 执行逻辑：1. 按起点再终点排序；2. 若新区间起点大于当前终点就开启新段；3. 否则扩展当前终点为两者最大值。
//   3. 为什么这样做：排序保证后续起点不下降；一旦出现间隔，未来区间也不可能再与已封闭段相交；重叠时取最大终点覆盖两段并集。
// - 边界与易错点：闭区间端点相等也算重叠，所以分段条件是 start>lastEnd；排序会修改输入。
// - 举一反三：区间并集、日程压缩、覆盖长度和扫描线预处理都从“排序使局部关系连续”这一点出发。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m (区间数量)。
//   接下来 m 行：start end。
//   每行输出一个合并后的区间 start end，并按起点升序排列。
// test.in 的预期输出：1 6 | 8 10 | 15 18
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<vector<int>> merge(vector<vector<int>>& intervals) {
        sort(intervals.begin(), intervals.end());
        vector<vector<int>> answer;
        for (const auto& interval : intervals) {
            if (answer.empty() || interval[0] > answer.back()[1]) {
                answer.push_back(interval);
            } else {
                answer.back()[1] = max(answer.back()[1], interval[1]);
            }
        }
        return answer;
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
    auto res = sol.merge(iv);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\n";
    return 0;
}

