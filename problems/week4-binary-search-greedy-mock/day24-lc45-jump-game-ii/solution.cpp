// ============================================================================
// LC-45：跳跃游戏 II
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 24 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个长度为 n 的 0 索引整数数组 nums。
// 初始位置在下标 0。
// 每个元素 nums[i] 表示从索引 i 向后跳转的最大长度。
// 如果你在索引 i 处，可以跳转到任意 i + j，其中 0 <= j <= nums[i] 且 i + j < n。
// 返回到达 n - 1 的最小跳跃次数。
// 测试用例保证可以到达 n - 1。
//
// 题目示例：
// 示例 1：
//   Input: nums = [2,3,1,1,4]
//   Output: 2
//   Explanation: The minimum number of jumps to reach the last index is 2. Jump 1 step from index 0 to 1, then 3 steps to the last index.
//
// 示例 2：
//   Input: nums = [2,3,0,1,4]
//   Output: 2
//
// 约束与要求：
//   - 1 <= nums.length <= 10^4
//   - 0 <= nums[i] <= 1000
//   - 题目保证可以到达 n - 1
//
// 复杂度目标：O(n) 时间，O(1) 空间（贪心）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜贪心分层：把 Jump Game 的可达前沿升级成隐式 BFS 层
//
// 0. 优化是怎么来的
//   一个自然且正确的做法是 BFS：下标是节点，从 i 到 i+1..i+nums[i] 都有一条边；BFS 第一次到达 n-1 的层数就是最少跳数。但显式枚举每个节点的所有下一跳，最坏会产生 O(n^2) 条边，而且队列里存着大量连续下标。
//
//   LC-55 已经告诉我们，这些可达节点往往能压成一个连续前沿。对最短跳数再保留一层信息即可：currentEnd 表示当前 BFS 层的右边界，farthest 表示扫描完整个当前层以后下一层的最右边界。层内不需要立刻决定‘到底从哪个位置跳’，因为在增加跳数之前，我们应该先比较这一层所有起点能把下一层推到哪里。
//
//   当扫描指针 i 到达 currentEnd，说明当前层所有候选都已考察完，此时才 jumps++ 并令 currentEnd=farthest。于是显式 BFS 队列和 O(n^2) 邻边枚举被两个边界变量替代。优化类型：BFS frontier 的区间压缩。
//
// 1. 图像直觉
//   nums = [2,3,1,1,4]
//
//   0 跳：
//   [0]
//   currentEnd = 0
//
//   扫描 i=0：farthest=max(0,0+2)=2
//   到达 currentEnd，进入第 1 层：
//   [1 2]
//     currentEnd=2
//
//   扫描这一整层：
//   i=1 -> farthest=max(2,1+3)=4
//   i=2 -> farthest 仍为 4
//
//   到达 currentEnd=2，才进入第 2 层：
//   [3 4]
//       currentEnd=4
//
//   因此最少 2 跳。
//
//   关键：在 i=1 时虽然已经看到能到 4，
//   也不能把每个 i 都算成一次新跳；
//   跳数按“层”增加，不按“扫描位置”增加。
//
// 2. 一句话核心
//   把一次跳跃当作 BFS 一层：扫描完当前层 [上一边界+1,currentEnd] 的所有起点后，才用 farthest 建立下一层并增加一次跳数。
//
// 3. 公式 / 不变量
//   jumps      = 已经确定的 BFS 层数
//   currentEnd = 使用 jumps 次跳跃能覆盖的当前层最右边界
//   farthest   = 扫描当前层期间，使用 jumps+1 次最多能覆盖的位置
//
//   对 i=0..n-2：
//       farthest = max(farthest, i + nums[i])
//       if i == currentEnd:
//           jumps += 1
//           currentEnd = farthest
//
//   不变量：在触发 i==currentEnd 之前，当前层的每个可达起点都被且只被扫描一次；farthest 汇总了这一整层所有下一跳的最大覆盖。
//
// 4. 执行步骤
//   1. 初始化 jumps=0、currentEnd=0、farthest=0，把下标 0 看作第 0 层
//   2. 扫描当前层的每个下标 i，用 i+nums[i] 更新 farthest，但此时不要立即增加 jumps
//   3. 只有当 i==currentEnd，才说明本层所有起点都已比较完；此时 jumps++，并令 currentEnd=farthest 进入下一层
//   4. 循环只到 n-2，因为一旦某层已经覆盖终点就不需要再从终点发起下一跳，避免多计一次
//   记忆：LC-55 的 farthest + 一个 currentEnd：层内只收集，层尾才跳一次。
//
// 5. 为什么不会漏 / 不会重
//   普通 BFS 在无权图中按层首次到达目标一定使用最少边数。本题从一层内所有连续可达下标出发，下一层的可达并集也是连续区间，其最右端就是这些 i+nums[i] 的最大值 farthest。因此可以只保存层边界而不保存队列中的每个节点。每次 jumps++ 前已扫描当前层所有可能起点，所以没有漏掉能让下一层更远的选择；而只有层结束才加一，也不会把同一次跳跃范围内的多个扫描位置重复计数。
//
// 6. 边界与易错点
//   不要每看到更大的 farthest 就 jumps++；跳数只在 i==currentEnd 时增加。循环到 n-2 而不是 n-1，否则长度大于 1 时到终点还可能多加一层。题目保证可达，所以现有实现无需额外处理 currentEnd 无法推进的死区；若迁移到无此保证的变体，应检测 farthest==currentEnd。
//
// 7. 举一反三
//   这是 LC-55 的直接升级：LC-55 只问最终前沿能否覆盖终点；LC-45 还要保留‘当前层边界’才能计最少步数。它本质上是隐式 BFS，可迁移到最少区间覆盖、最少加油次数等‘每一步能把连续前沿推进多远’的问题。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最少跳跃次数。
// test.in 的预期输出：2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int jump(vector<int>& nums) {
        // currentEnd 是“用当前 jumps 次跳跃能覆盖的这一层右边界”；
        // farthest 是扫描这一整层时，下一跳最多能把边界推进到哪里。
        int jumps = 0, currentEnd = 0, farthest = 0;

        // 不扫描最后一个下标：一旦进入终点就已经完成，若在终点再次触发层尾会多计一跳。
        for (int i = 0; i + 1 < static_cast<int>(nums.size()); ++i) {
            // 层内只收集所有起点的下一跳最远覆盖，不立即决定从哪个具体位置起跳。
            farthest = max(farthest, i + nums[i]);

            // 只有扫描完当前层的全部起点（i==currentEnd）时，才真正消耗一次跳跃，
            // 并把下一层边界整体更新为这一层汇总出的 farthest。
            if (i == currentEnd) {
                ++jumps;
                currentEnd = farthest;
            }
        }
        return jumps;
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
    cout << sol.jump(a) << "\n";
    return 0;
}
