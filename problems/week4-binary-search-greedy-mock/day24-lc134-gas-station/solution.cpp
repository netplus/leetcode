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
// 题目示例：
// 示例 1：
//   Input: gas = [1,2,3,4,5], cost = [3,4,5,1,2]
//   Output: 3
//   Explanation:
//   Start at station 3 (index 3) and fill up with 4 unit of gas. Your tank = 0 + 4 = 4
//   Travel to station 4. Your tank = 4 - 1 + 5 = 8
//   Travel to station 0. Your tank = 8 - 2 + 1 = 7
//   Travel to station 1. Your tank = 7 - 3 + 2 = 6
//   Travel to station 2. Your tank = 6 - 4 + 3 = 5
//   Travel to station 3. The cost is 5. Your gas is just enough to travel back to station 3.
//   Therefore, return 3 as the starting index.
//
// 示例 2：
//   Input: gas = [2,3,4], cost = [3,4,3]
//   Output: -1
//   Explanation:
//   You can't start at station 0 or 1, as there is not enough gas to travel to the next station.
//   Let's start at station 2 and fill up with 4 unit of gas. Your tank = 0 + 4 = 4
//   Travel to station 0. Your tank = 4 - 3 + 2 = 3
//   Travel to station 1. Your tank = 3 - 3 + 3 = 3
//   You cannot travel back to station 2, as it requires 4 unit of gas but you only have 3.
//   Therefore, you can't travel around the circuit once no matter where you start.
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
// 解法精讲｜贪心候选淘汰：一次失败排除整段起点
//
// 0. 优化是怎么来的
//   最直接的正确算法是把每个站都当起点模拟一整圈：从 s 出发逐站累加 gas[i]-cost[i]，一旦油量为负就判 s 失败，否则走满 n 站即成功。它显然不会漏答案，但最多 n 个起点各走 O(n) 站，最坏 O(n^2)。
//
//   浪费发生在失败模拟之间的大量重叠。设从 start 出发，直到 i 之前油量始终非负，而到 i 后第一次变负：
//   sum(start..i) < 0。
//   对于中间任意 j∈(start,i]，从 start 到 j-1 的累计因为此前未失败而 >=0，所以：
//   sum(j..i) = sum(start..i) - sum(start..j-1) <= sum(start..i) < 0。
//   也就是说 j 自己出发到 i 时会更早或同样没油，绝不可能成为答案。
//
//   因此一次失败可以把 start,start+1,...,i 全部永久排除，下一候选直接设为 i+1，不需要重新模拟这些起点。最后再用 total=sum(gain) 判断全局是否有足够总油量。优化类型：失败前缀的候选支配 / 批量淘汰。
//
// 1. 图像直觉
//   gas  = [1,2,3,4,5]
//   cost = [3,4,5,1,2]
//   gain = [-2,-2,-2,+3,+3]
//
//   start=0：
//   到 0：tank=-2 < 0
//   => 起点 0 失败，start=1
//
//   start=1：
//   到 1：tank=-2 < 0
//   => 起点 1 失败，start=2
//
//   start=2：
//   到 2：tank=-2 < 0
//   => 起点 2 失败，start=3
//
//   从 3 开始：
//   +3,+3,-2,-2,-2
//   前缀剩余：3,6,4,2,0
//   可以走完一圈。
//
//   更一般地，只要 start..i 第一次累计成负数：
//   start ........ i | i+1
//     整段起点都失败  ^ 下一个候选
//
// 2. 一句话核心
//   局部油箱一旦变负，就证明当前失败段里的所有起点都不可能成功；整段跳过，同时用全局净收益 total 判断整圈是否存在解。
//
// 3. 公式 / 不变量
//   gain[i] = gas[i] - cost[i]
//
//   total       += gain[i]
//   currentTank += gain[i]
//
//   若 currentTank < 0：
//       start = i + 1
//       currentTank = 0
//
//   为什么可整段淘汰：
//   在首次失败 i 前，对任意 j∈(start,i]：
//   sum(start..j-1) >= 0
//   而 sum(start..i) < 0
//   所以
//   sum(j..i)
//   = sum(start..i) - sum(start..j-1)
//   < 0。
//   故 j 也过不了 i。
//
//   全局必要且充分条件：
//   total = sum(gas)-sum(cost) >= 0。
//   扫描结束后若 total>=0，返回最后保留下来的 start；否则返回 -1。
//
// 4. 执行步骤
//   1. 顺序扫描每个站，计算 gain=gas[i]-cost[i]，同时累加全局 total 与当前候选的 currentTank
//   2. 只要 currentTank<0，就证明从当前 start 到 i 的所有起点都失败，把 start 直接跳到 i+1，并把局部油箱重置为 0
//   3. 继续从新候选累积；此前淘汰的起点永久不再回看，因此每个下标只处理一次
//   4. 扫描结束后检查 total：若 total<0，整圈总供给不足必然无解；否则返回最后的 start
//   记忆：局部失败淘汰一整段，全局 total 决定有没有解。
//
// 5. 为什么不会漏 / 不会重
//   每次局部失败时，上述前缀和关系证明 start..i 的所有起点都无法跨过 i，所以批量淘汰不会漏掉真正答案。扫描结束后，若 total<0，绕一圈最终消耗大于获得，无论从哪里出发都不可能成功。若 total>=0，所有被淘汰起点都已证明失败，而最后 start 到数组末尾的局部累计不会再变负；它从末尾绕回 0..start-1 时，所需的累计亏损由 total>=0 与此前失败段的总关系补足，因此该 start 可以完成整圈。
//
// 6. 边界与易错点
//   currentTank 只用于判断当前候选何时失败，不能代替 total；局部重置后 total 绝不能清零。净收益方向必须是 gas-cost。start 可能在某次失败后暂时变成 n，但若 total>=0，标准题设下最终可行候选会落在有效范围；现有约束和 int 范围也足以容纳总和，不过迁移到更大数值时可改 long long。
//
// 7. 举一反三
//   与 LC-53 最大子数组有共同结构：一个负贡献前缀面对相同未来只会拖累后续候选，因此可以丢弃；不同点是 LC-134 还必须满足环形全局守恒 total>=0。以后看到‘尝试每个起点会重复扫描，而某次失败能证明一整段起点都失败’，优先寻找这种批量淘汰证明。
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
        // total 记录整圈净收益，决定“是否存在解”；
        // currentTank 只记录当前候选 start 从起点走到 i 的局部剩余油量。
        int total = 0, currentTank = 0, start = 0;
        for (int i = 0; i < static_cast<int>(gas.size()); ++i) {
            const int gain = gas[i] - cost[i];
            total += gain;
            currentTank += gain;

            // 若 start..i 第一次累加成负数，不只是 start 失败：
            // 因为 start 到任意中间点此前前缀都非负，从其中任何 j∈(start,i] 重新出发，
            // 到 i 的净收益只会更差，因此这整段起点都可永久排除，下一候选直接跳到 i+1。
            if (currentTank < 0) {
                start = i + 1;
                currentTank = 0;
            }
        }

        // 局部失败时不能清空 total；只有整圈总油量不少于总消耗时才可能存在起点。
        // total>=0 时，所有早于 start 的候选已经被逐段证明失败，最后留下的 start 即可走完整圈。
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
