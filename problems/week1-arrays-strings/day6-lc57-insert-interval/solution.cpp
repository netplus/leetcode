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
// 题目示例：
// 示例 1：
//   Input: intervals = [[1,3],[6,9]], newInterval = [2,5]
//   Output: [[1,5],[6,9]]
//
// 示例 2：
//   Input: intervals = [[1,2],[3,5],[6,7],[8,10],[12,16]], newInterval = [4,8]
//   Output: [[1,2],[3,10],[12,16]]
//   Explanation: Because the new interval [4,8] overlaps with [3,5],[6,7],[8,10].
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
//
// 前置概念
//   本题沿用 LC-56 的闭区间语义：[L,R] 同时包含 L 和 R。
//
//   例如 [1,3] 与 [3,5] 在点 3 相交，所以必须合并；只有一段的 end 严格小于另一段的 start，才说明两段完全分离。
//
//   因此三阶段扫描中的边界分别是：
//   左侧：end < new.start
//   重叠：start <= new.end
//   右侧：start > new.end。
//
// 0. 优化是怎么来的
//   一个自然的复用方案，是把 newInterval 直接追加到 intervals，然后完整调用 LC-56：重新排序全部区间再线性合并，时间 O(n log n)，而且完全正确。
//
//   但本题额外给了比 LC-56 更强的条件：原 intervals 已经按 start 排序且互不重叠。重新排序等于把已经存在的顺序信息丢掉再计算一次。
//
//   利用这个条件，新区间只可能经历三个连续阶段：先经过所有完全在它左侧的区间；随后吸收一段与它相交的连续区间；一旦遇到 start>当前 new.end，后面所有区间都永久在右侧。于是无需排序，只做一次从左到右扫描。
//
//   优化类型：重复整理已有顺序 -> 复用输入已经建立的单调结构，把通用 LC-56 特化为 O(n) 三阶段插入。
//
// 1. 图像直觉
//   这题可以看成 LC-56 的“已整理时间线 + 插入一个新区间”。
//
//   intervals:   [1,2]   [3,5]   [6,7]   [8,10]   [12,16]
//   new:                    [4-----------8]
//
//   因为原区间已经有序且互不重叠，整条时间线天然分成三段：
//
//   左段：end < new.start
//   [1,2]
//   完全在新区间左边，原样复制。
//
//   中段：start <= new.end
//           [3,5] [6,7] [8,10]
//   它们会被新区间连续吸收：
//   new = [min(start), max(end)]
//   最终长成 [3,10]。
//
//   右段：start > new.end
//                                 [12,16]
//   完全在合并结果右边，原样复制。
//
//   关键是 newInterval 在中段不是固定的，它像雪球一样边走边扩大；扩大后的 new.end 可能继续吸收后面的区间。
//
// 2. 一句话核心
//   利用原数组已经有序且互不重叠，把扫描分成左边直接复制、中间让 newInterval 连续吸收、右边直接复制三段。
//
// 3. 公式 / 不变量
//   当前新区间 new = [L,R]，原区间 cur = [s,e]。
//
//   左段：e < L
//   cur 完全在 new 左边 -> 直接输出
//
//   中段：s <= R
//   cur 与 new 相交 ->
//   L = min(L,s)
//   R = max(R,e)
//
//   右段：s > R
//   后续也都在 new 右边 -> 先输出 new，再原样复制剩余区间
//
//   闭区间相交条件：
//   不是 (e < L 或 s > R)。
//
// 4. 执行步骤
//   1. 从头复制所有 intervals[i].end < newInterval.start 的左段
//   2. 随后只要 intervals[i].start <= newInterval.end，就把该区间吸收到 newInterval：同时更新左右端点
//   3. 中段结束时，把已经长成最终大小的 newInterval 放入 answer
//   4. 把剩余所有区间原样复制到 answer，形成右段
//   记忆：左边复制 -> 中间滚雪球吸收 -> 放下雪球 -> 右边复制。
//
// 5. 为什么不会漏 / 不会重
//   原 intervals 已按起点排序且彼此不重叠，因此完全位于 new 左侧的区间必然连续出现在最前面；进入第一个可能相交区间后，只要 start <= 当前不断扩大的 new.end，就属于同一个连通块，合并不会漏掉被扩张右端新触及的区间；一旦某区间 start > new.end，后续 start 更大，全部永久位于右侧。三段互斥且覆盖全部输入，所以不会漏也不会重。
//
// 6. 边界与易错点
//   这是闭区间，end == new.start 或 start == new.end 都算重叠；中段判断必须使用不断更新后的 newInterval.end，而不是最初的 end。当前实现会直接修改传入的 newInterval；LeetCode 允许这样做，但如果调用方需要保留原值，应改用局部副本。intervals 为空时仍要输出 newInterval。
//
// 7. 举一反三
//   知识迁移顺序是 LC-252 -> LC-56 -> LC-57：先学会排序后检测局部关系，再维护一个会增长的合并段，最后利用“输入已经排好”把全量 merge 特化成一次插入附近的局部 merge。这个三阶段扫描也常见于向有序事件流、版本区间或内存区段插入新范围。
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
