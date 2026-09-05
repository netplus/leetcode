// ============================================================================
// LC-416：分割等和子集
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 20 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个只包含正整数的非空数组 nums。
// 请你判断是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。
//
// 题目示例：
// 示例 1：
//   Input: nums = [1,5,11,5]
//   Output: true
//   Explanation: The array can be partitioned as [1, 5, 5] and [11].
//
// 示例 2：
//   Input: nums = [1,2,3,5]
//   Output: false
//   Explanation: The array cannot be partitioned into equal sum subsets.
//
// 约束与要求：
//   - 1 <= nums.length <= 200
//   - 1 <= nums[i] <= 100
//
// 复杂度目标：O(n*sum) 时间，O(sum) 空间（0/1 背包）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜0/1 背包可达性：把等分问题改写成每个数最多选一次的目标和
//
// 0. 优化是怎么来的
//   一个自然且正确的直接算法是对每个 nums[i] 做“选 / 不选”DFS，并维护当前子集和。走到末尾时检查是否等于 total/2；它枚举了所有下标子集，所以一定不会漏解。
//
//   问题在于不同选择路径会不断遇到相同的子问题。例如已经处理到下标 i、当前还需要凑 remaining 时，之前具体选了哪些元素并不再重要；未来能否成功只取决于“还剩哪些下标”和 remaining。直接 DFS 会把这些相同状态重复计算，最坏需要枚举 2^n 个子集。
//
//   先做一个更重要的建模化简。若两组和相等，总和 total 必须为偶数，并且其中任意一组的和都必须是 target=total/2。于是问题从“两组如何分配”变成：每个元素最多使用一次，是否能凑出 target。
//
//   可以用二维状态 reachable[i][s] 表示“只使用前 i 个元素能否凑出 s”。处理 value 时只有两类来源：不选它，继承 reachable[i-1][s]；选它，从 reachable[i-1][s-value] 转移。每个 (i,s) 只计算一次，就得到 O(n*target) DP。
//
//   再压成一维时，关键不只是省空间，而是保持‘读取上一层’的语义。若 s 从小到大更新，刚写出的 reachable[s-value] 可能已经使用了当前 value，随后又被本轮再次读取，相当于同一个下标重复使用。倒序从 target 到 value 更新，读取的 reachable[s-value] 仍是加入当前 value 之前的旧状态，因此天然保持 0/1 约束。
//
//   优化链：选/不选枚举所有子集 -> 相同 (处理位置, 剩余目标) 被重复求解 -> 等分化为 target=total/2 -> 每个状态只算一次 -> 观察只依赖上一层 -> 一维倒序 0/1 背包。
//
// 1. 图像直觉
//   nums = [1,5,11,5]，total=22，所以只需问：能否选出和为 11 的一组？
//
//   目标容量：0 1 2 ... 10 11
//   初始：    T F F ...  F  F
//
//   处理 value=1 后：
//   0 和 1 可达。
//
//   处理 value=5 时，想象把“旧可达集合”整体向右平移 5：
//   旧：{0,1}
//   加 5：{5,6}
//   合并：{0,1,5,6}
//
//   这里每个 value 对应的是一个具体数组下标，只允许做一次平移。
//
//   为什么一维必须倒序？假设这一轮 value=5：
//   若先把 reachable[5] 由 reachable[0] 设为 true，随后正序走到 10，又可能读取刚刚变 true 的 reachable[5]，于是同一个 5 被用了两次。
//   倒序时先算 10、再算 5，算 10 时 reachable[5] 仍是上一轮状态，不会偷用当前元素两次。
//
// 2. 一句话核心
//   先把等分改写成“是否存在和为 total/2 的下标子集”；每个元素只能选一次，所以一维 reachable 必须倒序更新，保证本轮只读上一层状态。
//
// 3. 公式 / 不变量
//   若 total 为奇数：答案必为 false。
//   否则 target = total / 2。
//
//   二维语义：
//   reachable[i][s] = 只使用前 i 个元素，是否能凑出和 s。
//
//   处理 value=nums[i-1]：
//   reachable[i][s]
//   = reachable[i-1][s]
//     OR (s>=value AND reachable[i-1][s-value])
//
//   一维初始化：
//   reachable[0] = true
//   其余为 false。
//
//   一维转移，容量倒序：
//   for value in nums:
//       for s = target ... value:
//           reachable[s] = reachable[s] || reachable[s-value]
//
//   循环不变量：开始处理某个 value 前，reachable[s] 精确表示“仅使用此前元素是否能凑出 s”；本轮倒序结束后，它精确加入了“当前 value 选 0 次或 1 次”的所有可能。
//
// 4. 执行步骤
//   1. 用 accumulate 计算 total；若 total 为奇数立即返回 false，因为两个整数子集不可能各占一半
//   2. 令 target=total/2，并建立 target+1 个布尔可达状态；reachable[0]=true 表示什么都不选可以凑出 0
//   3. 逐个处理 value；它对应一个具体数组下标，因此这一轮只能选择 0 次或 1 次
//   4. 容量 sum 从 target 倒序走到 value，并执行 reachable[sum] = reachable[sum] || reachable[sum-value]
//   5. 所有元素处理完后返回 reachable[target]；若它为真，未被该子集选中的剩余元素总和也必为 target
//   记忆：先砍一半：total/2；再问可达；元素只能一次，所以容量倒着走。
//
// 5. 为什么不会漏 / 不会重
//   对每个 value，任何使用已处理元素凑出 sum 的方案只有两类：不使用当前 value，此时旧 reachable[sum] 已完整覆盖；使用当前 value 一次，去掉它后必须由此前元素凑出 sum-value，因此由旧 reachable[sum-value] 完整覆盖。两类并集就是全部方案。倒序保证读取 sum-value 时该状态尚未被当前 value 更新，所以第二类确实只使用当前下标一次。最后若 target 可达，其补集总和为 total-target=target；反之任何等分方案都必包含一个和为 target 的子集，因此不会漏解。
//
// 6. 边界与易错点
//   最关键的错误是容量正序：那会把 0/1 背包悄悄改成可重复使用当前元素的完全背包。这里 nums 全为正数，target 范围可控；vector<char> 只是紧凑地保存布尔状态，不改变语义。总和奇偶判断必须在建 target 状态前完成。
//
// 7. 举一反三
//   这是 Day 18“选 / 不选”决策 DP 的集合版本，也是 Day 19 状态压缩思想的直接延伸。下一题 LC-494 会完整复用同一套 0/1 背包和倒序规则，只把 reachable 的值域从“是否可达”改成“有多少种下标子集”；LC-322 再通过遍历语义展示‘每件物品一次’与‘可无限复用’的根本区别。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：能分割为两个等和子集时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        const int total = accumulate(nums.begin(), nums.end(), 0);

        // 两组和相等时每组都必须是 total/2；总和为奇数时这个整数目标不存在，可立即判无解。
        if (total % 2 != 0) return false;
        const int target = total / 2;

        // reachable[sum] 表示：只使用已经处理过的数组下标，是否能让某个子集的和恰好为 sum。
        // 什么都不选恰好能组成 0，因此 reachable[0]=true 是整个 0/1 背包的起始状态。
        vector<char> reachable(target + 1, false);
        reachable[0] = true;

        for (int value : nums) {
            // 当前 value 对应一个具体下标，只允许使用 0 次或 1 次，所以容量必须倒序。
            // 倒序保证读取 reachable[sum-value] 时，它还没有被“当前 value”这一轮更新，
            // 因而该来源只使用此前元素；若正序，会可能读取本轮刚写出的状态而重复使用同一个 value。
            for (int sum = target; sum >= value; --sum) {
                // 不选当前 value：保留旧 reachable[sum]；
                // 选当前 value 一次：此前必须已经能凑出 sum-value。两类可能做 OR 合并。
                reachable[sum] = reachable[sum] || reachable[sum - value];
            }
        }

        // target 可达时，所选子集和为 total/2，其补集自然也是 total/2。
        return reachable[target];
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.canPartition(a) ? 1 : 0) << "\n";
    return 0;
}

