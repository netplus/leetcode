// ============================================================================
// LC-134：加油站
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 4 周 / 第 24 天
// ----------------------------------------------------------------------------
// 题目描述：
// 在一条环路上有 n 个加油站，其中第 i 个加油站有汽油 gas[i] 升。
// 你有一辆油箱容量无限的的汽车，从第 i 个加油站开往第 i+1 个加油站需要消耗汽油 cost[i] 升。
// 你从其中的一个加油站出发，开始时油箱为空。
// 给定两个整数数组 gas 和 cost，如果你可以按顺序绕环路行驶一周，则返回出发时加油站的编号，否则返回 -1。
// 如果存在解，则保证它是唯一的。
//
// 约束与要求：
//   - n == gas.length == cost.length
//   - 1 <= n <= 10^5
//   - 0 <= gas[i], cost[i] <= 10^4
//   - 输入保证答案唯一。
//
// 复杂度目标：O(n) 时间，O(1) 空间（贪心）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜贪心重置候选起点 + 全局可行性
// - 核心要点：
//   1. 思路起点：总油量小于总消耗时无解；否则扫描净收益，若从当前 start 到 i 的油箱变负，则这一段内任何位置都不可能作为起点，候选重置为 i+1。
//   2. 执行逻辑：1. 累计 total 与 currentTank；2. currentTank<0 时重置 start 和 currentTank；3. 扫描后 total>=0 返回 start，否则 -1。
//   3. 为什么这样做：若 start..i 总和为负，则其中任一更晚起点拥有的初始累计不可能优于 start 在到达它时的非负余量，因此都无法跨过 i；可一次排除整段。
// - 边界与易错点：局部重置不能替代全局 total 检查；答案按题意唯一；净收益计算为 gas[i]-cost[i]。
// - 举一反三：环形可行起点、前缀亏损重置和最大子数组都有“负前缀只会拖累后续”的共同思想。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个 gas 值。
//   第 3 行：n 个 cost 值。
//   输出：可行的起点下标；不存在时输出 -1。
// test.in 的预期输出：3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int canCompleteCircuit(vector<int>& gas, vector<int>& cost) {
        int total = 0, currentTank = 0, start = 0;
        for (int i = 0; i < static_cast<int>(gas.size()); ++i) {
            const int gain = gas[i] - cost[i];
            total += gain;
            currentTank += gain;
            if (currentTank < 0) {
                start = i + 1;
                currentTank = 0;
            }
        }
        return total >= 0 ? start : -1;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> gas(n), cost(n);
    for (int i = 0; i < n; ++i) cin >> gas[i];
    for (int i = 0; i < n; ++i) cin >> cost[i];
    cout << sol.canCompleteCircuit(gas, cost) << "\n";
    return 0;
}

