"""Reviewed C++ key-code comments for Week 3 / Day 20.

Entries are added only after an independent per-problem review.
"""


CODE_COMMENTS_WEEK3_DAY20 = {
    416: r'''// ---------- Solution ----------
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
};''',

    494: r'''// ---------- Solution ----------
class Solution {
public:
    int findTargetSumWays(vector<int>& nums, int target) {
        const int total = accumulate(nums.begin(), nums.end(), 0);

        // 设加正号元素和为 P、负号元素和为 N：P-N=target，P+N=total，
        // 因而 P=(total+target)/2。绝对目标超过 total 或右侧为奇数时，不存在整数子集和解。
        if (abs(target) > total || (total + target) % 2 != 0) return 0;
        const int positiveSum = (total + target) / 2;

        // ways[sum] 表示：只使用已经处理过的数组下标，和恰好为 sum 的下标子集数量。
        // ways[0]=1 对应“空子集”这一种基础方案；没有这个 1，后续任何 value 都无法从 0 建出第一批方案。
        vector<long long> ways(positiveSum + 1, 0);
        ways[0] = 1;

        for (int value : nums) {
            // 与 LC-416 完全相同：每个数组下标只能分到正号集合 0/1 次，所以 sum 必须倒序。
            // 这样 ways[sum-value] 在读取时仍只统计此前下标，不会在同一轮重复使用当前 value。
            for (int sum = positiveSum; sum >= value; --sum) {
                // 旧 ways[sum] 是“不选当前下标”的方案；ways[sum-value] 是“选当前下标”的方案。
                // 当 value==0 时，这行变成 ways[sum]+=ways[sum]，恰好把每个方案翻倍，
                // 对应同一个 0 可以分别选择 +0 或 -0，两种表达式确实不同。
                ways[sum] += ways[sum - value];
            }
        }

        // 每个和为 positiveSum 的正号下标集合，都唯一决定其余下标放负号，因此计数与目标表达式一一对应。
        return static_cast<int>(ways[positiveSum]);
    }
};''',

    322: r'''// ---------- Solution ----------
class Solution {
public:
    int coinChange(vector<int>& coins, int amount) {
        // amount+1 是安全的“不可能”哨兵：若 amount 可由正面额硬币组成，
        // 最差也只需 amount 枚面额 1 的硬币，因此任何有效最少枚数都不会达到 amount+1。
        vector<int> dp(amount + 1, amount + 1);

        // 凑出金额 0 不需要任何硬币，这是所有正金额转移的唯一基础可达状态。
        dp[0] = 0;

        // 金额从小到大，保证处理 current 时所有 current-coin 都是已经求好的更小金额状态。
        for (int current = 1; current <= amount; ++current) {
            for (int coin : coins) {
                // 把 coin 看作最优方案的最后一枚硬币：若 coin<=current，
                // 候选就是“凑出 current-coin 的最少枚数 + 当前这一枚”。
                // 同一面额可以无限使用，所以 dp[current-coin] 即使已经包含 coin 也完全合法；
                // 这正是与 LC-416/494 的 0/1 倒序语义不同之处。
                if (coin <= current) dp[current] = min(dp[current], dp[current - coin] + 1);
            }
        }

        // 哨兵未被改善说明 amount 不可达；否则 dp[amount] 就是最少硬币枚数。
        return dp[amount] == amount + 1 ? -1 : dp[amount];
    }
};''',
}
