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
//   一个自然且正确的做法是 BFS：把下标看成节点，从 i 到 i+1..i+nums[i] 都有边；
//   BFS 第一次到达 n-1 的层数就是最少跳数。问题是显式枚举这些边最坏会达到 O(n^2)，
//   队列中还会保存大量连续下标。
//
//   LC-55 已经告诉我们，可达节点可以压缩成连续前沿。这里再保留一层信息：
//   currentEnd 表示当前 BFS 层的右边界，farthest 表示扫描完整个当前层后下一层能到的最远点。
//   层内不需要立刻决定“从哪个点跳”，应该先比较这一层所有起点能把下一层推到哪里。
//
//   因此显式 BFS 队列与大量边，被 currentEnd + farthest 两个边界变量替代。
//
// 1. 图像直觉
//   nums = [2,3,1,1,4]
//
//   0 跳： [0]，currentEnd=0
//   扫描 i=0：farthest=2
//   到达 currentEnd，进入第 1 层：[1 2]
//
//   扫描整层：
//   i=1 -> farthest=4
//   i=2 -> farthest 仍为 4
//   到达 currentEnd=2，才进入第 2 层：[3 4]
//
//   所以最少 2 跳。关键是：跳数按“层”增加，不按“扫描了几个位置”增加。
//
// 2. 一句话核心
//   把一次跳跃看成 BFS 一层：扫描完当前层的所有起点后，才用 farthest 建立下一层并把 jumps 加一。
//
// 3. 公式 / 不变量
//   jumps      = 已经确定的 BFS 层数
//   currentEnd = 使用 jumps 次跳跃能覆盖的当前层最右边界
//   farthest   = 扫描当前层时，使用 jumps+1 次最多能覆盖的位置
//
//   对 i=0..n-2：
//       farthest = max(farthest, i + nums[i])
//       if i == currentEnd:
//           jumps += 1
//           currentEnd = farthest
//
//   不变量：触发 i==currentEnd 前，当前层的所有可达起点都恰好扫描一次；
//   farthest 汇总了这一整层所有下一跳的最大覆盖。
//
// 4. 执行步骤
//   1. 初始化 jumps=0、currentEnd=0、farthest=0，把下标 0 看作第 0 层。
//   2. 扫描当前层每个 i，用 i+nums[i] 更新 farthest，但层内不要增加 jumps。
//   3. 只有 i==currentEnd 时，本层才算扫描完整；此时 jumps++，并令 currentEnd=farthest。
//   4. 循环只到 n-2，避免已经到终点后还多计一次跳跃。
//   记忆：LC-55 的 farthest + 一个 currentEnd：层内只收集，层尾才跳一次。
//
// 5. 为什么不会漏 / 不会重
//   普通 BFS 在无权图中按层首次覆盖目标一定使用最少边数。本题同一层中所有可达下标组成连续区间，
//   它们下一跳的并集也能用最右端 farthest 表示。因此可以只保存层边界而不保存队列中的每个节点。
//   每次增加 jumps 前已经考察当前层所有起点，不会漏掉更远选择；只有层结束才加一，也不会重复计数。
//
// 6. 边界与易错点
//   不要每看到更大的 farthest 就 jumps++；跳数只在 i==currentEnd 时增加。
//   循环到 n-2 而不是 n-1；长度 1 的答案自然为 0。题目保证终点可达，所以无需额外处理死区。
//
// 7. 举一反三
//   这是 LC-55 的直接升级：LC-55 只问前沿最终能否覆盖终点，LC-45 还要保留“当前层边界”来计算最少步数。
//   这种隐式 BFS 也常出现在最少区间覆盖、最少加油次数等“每一步能把连续前沿推进多远”的问题中。
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
