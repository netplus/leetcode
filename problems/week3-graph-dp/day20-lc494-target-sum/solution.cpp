// ============================================================================
// LC-494：目标和
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 20 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个非负整数数组 nums 和一个整数 target。
// 向数组中的每个整数前添加 '+' 或 '-'，然后串联起所有整数，可以构造一个表达式：例如，nums = [2, 1]，可以在 2 之前添加 '+'，在 1 之前添加 '-'，然后串联起来得到表达式
// "+2-1"。
// 返回可以通过上述方法构造的、运算结果等于 target 的不同表达式的数目。
//
// 题目示例：
// 示例 1：
//   Input: nums = [1,1,1,1,1], target = 3
//   Output: 5
//   Explanation: There are 5 ways to assign symbols to make the sum of nums be target 3.
//   -1 + 1 + 1 + 1 + 1 = 3
//   +1 - 1 + 1 + 1 + 1 = 3
//   +1 + 1 - 1 + 1 + 1 = 3
//   +1 + 1 + 1 - 1 + 1 = 3
//   +1 + 1 + 1 + 1 - 1 = 3
//
// 示例 2：
//   Input: nums = [1], target = 1
//   Output: 1
//
// 约束与要求：
//   - 1 <= nums.length <= 20
//   - 0 <= nums[i] <= 1000
//   - 0 <= sum(nums[i]) <= 1000
//   - -1000 <= target <= 1000
//
// 复杂度目标：O(n*sum) 时间（0/1 背包）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜0/1 背包计数：先用代数把正负号选择变成 LC-416 的子集和
//
// 0. 优化是怎么来的
//   最自然的正确算法是 DFS：来到 nums[i] 时分别尝试 +nums[i] 和 -nums[i]，处理完全部元素后若累计和等于 target 就给答案加一。每个表达式对应一条唯一根到叶路径，因此算法完全正确，但会枚举 2^n 种符号选择。
//
//   可以先做记忆化，把状态压成 (i,currentSum)，因为走到同一 i 且累计和相同时，后面的选择完全相同。这已经消除了重复子问题。但这题还有更强的结构：所有 nums[i] 都是非负数，正负号实际上是在把下标分成两个集合。
//
//   设加正号的元素和为 P，加负号的元素和为 N：
//   P - N = target
//   P + N = total
//   两式相加得到 2P = total + target，因此 P=(total+target)/2。
//
//   所以只要 |target|<=total 且 total+target 为偶数，原问题就等价为：从每个下标最多选一次，有多少个下标子集的元素和恰好为 positiveSum。这正是 LC-416 的 0/1 背包状态图，只是 LC-416 做 OR，而这里做加法计数。
//
//   于是 ways[s] 表示使用已处理下标凑出 s 的方案数。处理 value 时倒序执行 ways[s]+=ways[s-value]。倒序仍负责‘每个下标只使用一次’；加法则把不同子集的方案数汇总起来。
//
//   优化链：2^n 正负 DFS -> 重复的 (i,sum) -> 用 P/N 方程把符号问题转成子集和 -> 复用 LC-416 的 0/1 背包 -> 布尔 OR 改成方案数相加。
//
// 1. 图像直觉
//   nums=[1,1,1,1,1], target=3，total=5。
//
//   P - N = 3
//   P + N = 5
//   ----------
//   2P = 8
//   P = 4
//
//   所以不必继续想“每个位置放 + 还是 -”，只需问：有多少个下标子集的和为 4？
//   五个 1 中任意挑四个，共 5 种；这正对应官方答案 5。
//
//   把 LC-416 的状态做一个替换：
//
//   LC-416：reachable[s] = 能 / 不能
//   LC-494：ways[s]      = 有多少种
//
//   状态转移拓扑完全一样：
//   旧 ways[s]          -> 不选当前 value
//   旧 ways[s-value]    -> 选当前 value
//   两类方案互斥，所以数量直接相加。
//
//   value=0 时也没有例外：
//   ways[s] += ways[s]
//   会把每个已有方案翻倍，恰好对应这个 0 可以写成 +0 或 -0，两种表达式不同。
//
// 2. 一句话核心
//   先用 P-N=target、P+N=total 把符号选择变成和为 (total+target)/2 的下标子集计数，再原样复用 LC-416 的 0/1 倒序状态。
//
// 3. 公式 / 不变量
//   设：
//   P = 加 '+' 的元素和
//   N = 加 '-' 的元素和
//   total = P + N
//
//   要求：
//   P - N = target
//   因此：
//   2P = total + target
//   positiveSum = (total + target) / 2
//
//   无解条件：
//   abs(target) > total
//   或 total + target 为奇数。
//
//   状态：
//   ways[s] = 使用已经处理的下标，和恰好为 s 的子集数量。
//
//   初始化：
//   ways[0] = 1
//
//   0/1 倒序转移：
//   for value in nums:
//       for s = positiveSum ... value:
//           ways[s] += ways[s-value]
//
//   不变量：每轮开始前 ways 只统计此前下标；倒序结束后，所有方案恰好把当前下标分成“不选”和“选一次”两类。
//
// 4. 执行步骤
//   1. 计算 total；若 abs(target)>total，目标绝对值已经超出所有数字可提供的总量，直接返回 0
//   2. 检查 total+target 的奇偶；若为奇数，positiveSum 不是整数，不可能存在对应符号划分
//   3. 计算 positiveSum=(total+target)/2，建立 ways 数组并令 ways[0]=1，代表空子集这一种基础方案
//   4. 逐个处理 value，并和 LC-416 一样让容量从 positiveSum 倒序到 value，保证每个数组下标只参与一次
//   5. 执行 ways[sum]+=ways[sum-value]；最终 ways[positiveSum] 就是表达式数量
//   记忆：先列两式求 P；然后把 LC-416 的 true/false 换成 ways 数量，倒序规则一字不改。
//
// 5. 为什么不会漏 / 不会重
//   每个完整符号表达式把所有下标唯一划分为正号集合和负号集合；由两条方程可知它满足目标和，当且仅当正号下标集合的元素和为 positiveSum。反过来，任意这样的下标子集都唯一决定哪些位置放 '+'，其余位置放 '-'，因此二者是一一对应。0 虽然数值不改变集合和，但不同下标是否进入正号集合仍对应 +0/-0 的不同表达式，DP 会正确计数。倒序转移对每个下标只选 0/1 次，所以不会把同一下标重复计入。
//
// 6. 边界与易错点
//   必须判断 abs(target)>total，而不是只判断 target>total，因为 target 可以为负。还必须检查 total+target 的奇偶。ways 保存的是方案数而不是可达性；遇到 0 时翻倍是正确行为，不能去重。当前实现用 long long 保存中间计数，再按题目返回 int。
//
// 7. 举一反三
//   这是 LC-416 最直接的模型迁移：状态空间、倒序方向、每个元素一次的约束全部不变，只把布尔半环的 OR 改成整数加法。看到‘正负号 / 两组分配’时，先列总和与差值方程，常常可以先降维成子集和，再决定是求可达、计数还是最值。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的整数。
//   输出：计算结果等于 target 的表达式数量。
// test.in 的预期输出：5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        const int total = accumulate(nums.begin(), nums.end(), 0);
        if (abs(target) > total || (total + target) % 2 != 0) return 0;
        const int positiveSum = (total + target) / 2;
        vector<long long> ways(positiveSum + 1, 0);
        ways[0] = 1;
        for (int value : nums) {
            for (int sum = positiveSum; sum >= value; --sum) {
                ways[sum] += ways[sum - value];
            }
        }
        return static_cast<int>(ways[positiveSum]);
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k;
    if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.findTargetSumWays(a, k) << "\n";
    return 0;
}

