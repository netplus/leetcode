// ============================================================================
// LC-1248：统计「优美子数组」
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 1 周 / 第 1 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums 和一个整数 k。
// 如果某个连续子数组中恰好有 k 个奇数数字，我们就认为这个子数组是「 优美子数组 」。
// 请返回这个数组中 「优美子数组」 的数目。
//
// 约束与要求：
//   - 1 <= nums.length <= 50000
//   - 1 <= nums[i] <= 10^5
//   - 1 <= k <= nums.length
//
// 复杂度目标：O(n) 时间，O(1) 额外空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜恰好型计数：atMost(k) - atMost(k-1)
// - 核心要点：
//   1. 思路起点：把奇数视为 1、偶数视为 0；“恰好 k 个奇数”可由“至多 k 个”减去“至多 k-1 个”得到。
//   2. 执行逻辑：1. 写滑动窗口函数 atMost(limit)；2. 右端加入一个数并在奇数超限时收缩左端；3. 每个右端贡献 right-left+1，再作两次结果之差。
//   3. 为什么这样做：固定右端时，窗口恢复合法后，以 right 结尾且奇数数目至多 limit 的起点正是 [left,right]；两集合做差只保留恰好 k 个。
// - 边界与易错点：atMost(-1) 必须返回 0；答案可能由大量子数组累积，内部使用 long long；偶数不能被误当作窗口边界。
// - 举一反三：恰好 K 个不同整数、恰好 K 次违规、二进制数组和为 K 都可套用 atMost 差分。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n k。
//   第 2 行：n 个以空格分隔的整数。
//   输出：优美子数组的数量。
// test.in 的预期输出：2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    static long long atMost(const vector<int>& nums, int limit) {
        if (limit < 0) return 0;
        long long count = 0;
        int left = 0;
        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            limit -= nums[right] & 1;
            while (limit < 0) limit += nums[left++] & 1;
            count += right - left + 1;  // 这些起点都能形成合法窗口
        }
        return count;
    }

public:
    int numberOfSubarrays(vector<int>& nums, int k) {
        return static_cast<int>(atMost(nums, k) - atMost(nums, k - 1));
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
    cout << sol.numberOfSubarrays(a, k) << "\n";
    return 0;
}

