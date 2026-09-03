// ============================================================================
// LC-410：分割数组的最大值
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 23 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个非负整数数组 nums 和一个整数 k，你需要将这个数组分成 k 个非空的连续子数组，使得这 k 个子数组各自和的最大值最小。
// 返回分割后最小的和的最大值。
// 子数组是数组中连续的部分。
//
// 约束与要求：
//   - 1 <= nums.length <= 1000
//   - 0 <= nums[i] <= 10^6
//   - 1 <= k <= min(50, nums.length)
//
// 复杂度目标：O(n log S) 其中 S = sum(nums)。
//
// ----------------------------------------------------------------------------
// 解法精讲｜答案二分 + 贪心可行性检查
// - 核心要点：
//   1. 思路起点：目标最大子数组和越大，所需分段数单调不增；给定上限 limit，按顺序尽量装满每段能得到最少段数。
//   2. 执行逻辑：1. 搜索下界=max(nums)、上界=sum(nums)；2. check(limit) 贪心统计需要几段；3. 段数<=k 说明可行并收缩右界，否则提高左界。
//   3. 为什么这样做：正数条件下，当前元素放不进本段时切段是必要且最晚的，因此得到给定上限的最少段数；可行谓词随 limit 单调。
// - 边界与易错点：子数组必须连续且非空；上下界和中点用 long long；允许最多 k 段等价于能通过继续拆分得到恰好 k 段。
// - 举一反三：最小化最大负载、工人分配、磁带切分和运载容量都常用“猜答案 + 贪心判定”。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n k。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最小化后的最大子数组和。
// test.in 的预期输出：18
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        long long left = *max_element(nums.begin(), nums.end());
        long long right = accumulate(nums.begin(), nums.end(), 0LL);

        auto groupsNeeded = [&](long long limit) {
            int groups = 1;
            long long current = 0;
            for (int value : nums) {
                if (current + value > limit) {
                    ++groups;
                    current = 0;
                }
                current += value;
            }
            return groups;
        };

        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (groupsNeeded(middle) <= k) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
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
    cout << sol.splitArray(a, k) << "\n";
    return 0;
}

