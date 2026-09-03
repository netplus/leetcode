// ============================================================================
// LC-57：插入区间
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 1 周 / 第 6 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个无重叠的，按照区间起始端点排序的区间列表 intervals，其中 intervals[i] = [start_i, end_i] 表示第 i 个区间的开始和结束，并且 intervals 按照
// start_i 升序排列。
// 同样给定一个区间 newInterval = [start, end] 表示另一个区间的开始和结束。
// 如果两个区间至少共享一个点，则认为它们是重叠的。
// 在 intervals 中插入区间 newInterval，使得 intervals 依然按照 start_i 升序排列，且区间之间不重叠（如果有必要的话，可以合并区间）。
// 返回插入之后的 intervals。
// 注意你不需要原地修改 intervals。
// 你可以创建一个新数组然后返回它。
//
// 约束与要求：
//   - 0 <= intervals.length <= 10^4
//   - intervals[i].length == 2
//   - 0 <= start_i <= end_i <= 10^5
//   - intervals 根据 start_i 按升序排列
//   - newInterval.length == 2
//   - 0 <= start <= end <= 10^5
//
// 复杂度目标：O(n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜有序区间的三阶段扫描
// - 核心要点：
//   1. 思路起点：原区间已按起点排序且互不重叠，可分为完全在新区间左边、与新区间相交、完全在右边三段。
//   2. 执行逻辑：1. 复制 end<newStart 的左段；2. 吸收所有 start<=newEnd 的重叠段并扩大新区间；3. 放入合并段后复制剩余右段。
//   3. 为什么这样做：三类按排序顺序连续且互斥；中段全部与不断扩大的新区间相交，合并后覆盖其并集，左右段保持原有顺序。
// - 边界与易错点：端点相接视为重叠；intervals 可为空；题目不要求原地修改，直接构造结果更清晰。
// - 举一反三：已排序数据中插入并局部合并的模式，也用于事件时间线、版本范围和内存区段管理。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m (区间数量)。
//   接下来 m 行：start end。
//   最后一行：start end (新区间)。
//   每行输出一个合并后的区间 start end。
// test.in 的预期输出：1 2 | 3 10
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<vector<int>> insert(vector<vector<int>>& intervals, vector<int>& newInterval) {
        vector<vector<int>> answer;
        int i = 0, n = static_cast<int>(intervals.size());

        while (i < n && intervals[i][1] < newInterval[0]) {
            answer.push_back(intervals[i++]);
        }
        while (i < n && intervals[i][0] <= newInterval[1]) {
            newInterval[0] = min(newInterval[0], intervals[i][0]);
            newInterval[1] = max(newInterval[1], intervals[i][1]);
            ++i;
        }
        answer.push_back(newInterval);
        while (i < n) answer.push_back(intervals[i++]);
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
    vector<int> ni(2); cin >> ni[0] >> ni[1];
    auto res = sol.insert(iv, ni);
    for (auto& x : res) cout << x[0] << " " << x[1] << "\n";
    return 0;
}

