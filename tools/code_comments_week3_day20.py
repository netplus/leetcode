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
}
