// ============================================================================
// LC-312：戳气球
// 难度：困难
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 20 天
// ----------------------------------------------------------------------------
// 题目描述：
// 有 n 个气球，编号为 0 到 n - 1，每个气球上都标有一个数字，这些数字存在数组 nums 中。
// 现在要求你戳破所有的气球。
// 戳破第 i 个气球，你可以获得 nums[i - 1] * nums[i] * nums[i + 1] 枚硬币。
// 这里的 i - 1 和 i + 1 代表和 i 相邻的两个气球的序号。
// 如果 i - 1 或 i + 1 超出了数组的边界，那么就当它是一个数字为 1 的气球。
// 求所能获得硬币的最大数量。
//
// 约束与要求：
//   - n == nums.length
//   - 1 <= n <= 300
//   - 0 <= nums[i] <= 100
//
// 复杂度目标：O(n^3) 时间（区间 DP）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜区间 DP：枚举区间内最后戳破的气球
// - 核心要点：
//   1. 思路起点：给两端补值 1；dp[left][right] 表示开区间 (left,right) 内全部戳完的最大收益，枚举最后被戳的 k，此时它的邻居确定为 left/right。
//   2. 执行逻辑：1. 构造带边界 1 的 values；2. 按开区间长度从小到大；3. 枚举 k，转移 dp[left][k]+values[left]*values[k]*values[right]+dp[k][right]。
//   3. 为什么这样做：虽然第一枚气球邻居难确定，但最后一枚的邻居必是区间边界；所有戳破顺序按其最后元素唯一分类，左右子区间彼此独立。
// - 边界与易错点：状态是开区间，长度至少 2 才有内部元素；遍历区间长度必须保证子区间已计算；不要贪心当前最大收益。
// - 举一反三：区间 DP 常通过枚举“最后合并/最后操作/分割点”解除动态邻接，例如矩阵链乘、石子合并和多边形三角剖分。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：可获得的最大硬币数。
// test.in 的预期输出：167
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int maxCoins(vector<int>& nums) {
        vector<int> values;
        values.reserve(nums.size() + 2);
        values.push_back(1);
        values.insert(values.end(), nums.begin(), nums.end());
        values.push_back(1);
        const int n = static_cast<int>(values.size());
        vector<vector<int>> dp(n, vector<int>(n, 0));

        for (int width = 2; width < n; ++width) {
            for (int left = 0; left + width < n; ++left) {
                int right = left + width;
                for (int last = left + 1; last < right; ++last) {
                    dp[left][right] = max(dp[left][right],
                        dp[left][last] + values[left] * values[last] * values[right]
                        + dp[last][right]);
                }
            }
        }
        return dp[0][n - 1];
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
    cout << sol.maxCoins(a) << "\n";
    return 0;
}

