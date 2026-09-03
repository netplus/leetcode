"""High-touch learning rewrites for individually reviewed problems.

The week modules remain the baseline reviewed solution registry.  This file stores
only the problems that have received the stronger learning pass defined in
AGENTS.md: visual intuition -> one-sentence core -> formula/invariant -> steps.

Keeping these overrides separate lets us improve the 106-problem workspace one
problem at a time without mechanically rewriting untouched material.  gen_all.py
consumes the merged registry through refined_data.py, so regenerated solution.cpp
files keep the learning rewrites.
"""

PEDAGOGY_OVERRIDES = {
    1: {
        "visual": """例：nums = [2, 7, 11, 15], target = 9。扫描到 7 时：

历史                当前
[ 2 ]                7
  ^                  |
  +---- 2 = 9 - 7 ---+

当前数只需要到“已经看过的历史”里找自己的补数。""",
        "core": "当前拿到 x，不去未来枚举配对，只问历史里是否已经出现 target-x。",
        "formula": """x + old = target
old = target - x""",
        "memory": "先查补数 -> 命中就返回 -> 未命中再记录当前值。",
    },

    53: {
        "visual": """走到当前位置 x 时，只比较两种连续子数组：

... [前一位置的最佳后缀] | x
          current          ^

继续接上：current + x
从这里重开：       x

旧后缀如果已经是负担，就没有必要继续带着它。""",
        "core": "走到每个位置，只决定一件事：前面的最佳后缀值得不值得继续带上。",
        "formula": """current = max(nums[i], current + nums[i])
best    = max(best, current)

其中 current 始终表示“必须以 i 结尾”的最大子数组和。""",
        "memory": "每个位置先选“接上还是重开”，再用这个位置的最佳答案更新全局 best。",
    },

    560: {
        "visual": """把两个前缀和画成两段累计长度：

|---------------- 当前 prefix ----------------|
|---------- 历史 prefix-k ----------|
                                      ^
                              中间连续区间的和恰好为 k

所以并不是直接枚举子数组，而是在当前累计值下寻找一个合适的历史累计值。""",
        "core": "我现在累计到了 prefix，历史上有多少次累计到了 prefix-k？",
        "formula": """当前累计 - 过去累计 = 中间连续区间的和
prefix - oldPrefix = k
oldPrefix = prefix - k""",
        "memory": "先算现在 -> 再查过去 -> 最后把现在加入历史。",
    },

    1248: {
        "pattern": "奇偶映射 + 前缀和 + 频次哈希表（复用 LC-560）",
        "model": "先把奇数记为 1、偶数记为 0；这样“连续子数组中恰好有 k 个奇数”就直接变成“连续子数组和为 k”，可以复用 LC-560。",
        "steps": [
            "frequency[0]=1，记录数组开始前的空前缀",
            "扫描 value：奇数让 prefix 加 1，偶数让 prefix 加 0",
            "用 find(prefix-k) 查询历史频次，存在就累加到答案",
            "最后执行 ++frequency[prefix]，把当前累计奇数数加入历史",
        ],
        "proof": "prefix 表示从开头到当前位置累计遇到的奇数个数；两个 prefix 的差就是它们之间连续区间的奇数个数，因此历史 prefix-k 与当前位置一一组成恰好含 k 个奇数的子数组。",
        "pitfalls": "必须保存 prefix 的出现频次而不是只判断存在；frequency[0]=1 才能统计从下标 0 开始的答案；查询 prefix-k 用 find()，避免纯查询意外插入；偶数只是贡献 0，不是窗口边界。主解法 O(n) 额外空间。",
        "transfer": "这是 LC-560 的直接变形：先把题目属性映射成可累加的 0/1 状态，再统计指定前缀差；类似的“恰好 K 次满足条件”问题也可先尝试这种转换。",
        "visual": """先做一次非常简单的等价转换：

原数组： [2, 2, 1, 2, 1, 2]
奇偶值： [0, 0, 1, 0, 1, 0]

于是问题变成：上面的 0/1 数组中，有多少个连续子数组的和为 k？

|------------- 当前 prefix：累计奇数数 -------------|
|--------- 历史 prefix-k ---------|
                                      ^
                              中间恰好多出 k 个奇数

这和 LC-560 的图完全相同，只是 prefix 从“元素累计和”变成了“奇数累计个数”。""",
        "core": "先把奇数当 1、偶数当 0，然后把这题直接当成 LC-560：当前 prefix 去历史里找 prefix-k。",
        "formula": """区间内奇数个数 = 当前累计奇数数 - 过去累计奇数数
prefix - oldPrefix = k
oldPrefix = prefix - k""",
        "memory": "奇偶转 0/1 -> 先算现在 -> 查 prefix-k -> 再记录现在。",
        "advanced": "如果必须满足题头的 O(1) 额外空间目标，再学习 atMost(k)-atMost(k-1) 的滑动窗口版本；它是空间优化，不作为第一次理解本题的主线。",
        "code": r'''// ---------- Solution ----------
class Solution {
public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        unordered_map<int, int> frequency;
        frequency.reserve(nums.size() * 2 + 1);
        frequency[0] = 1;  // 空前缀：还没看任何元素时，累计奇数数为 0

        int prefix = 0;
        int answer = 0;
        for (int value : nums) {
            prefix += value & 1;  // 奇数 -> 1，偶数 -> 0

            // 纯查询：历史上有多少次累计奇数数等于 prefix-k？
            auto it = frequency.find(prefix - k);
            if (it != frequency.end()) {
                answer += it->second;
            }

            // 查询完成后，再把“现在”加入历史，供后续位置使用。
            ++frequency[prefix];
        }
        return answer;
    }
};''',
    },
}
