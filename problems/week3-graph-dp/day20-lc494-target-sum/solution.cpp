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
// 约束与要求：
//   - 1 <= nums.length <= 20
//   - 0 <= nums[i] <= 1000
//   - 0 <= sum(nums[i]) <= 1000
//   - -1000 <= target <= 1000
//
// 复杂度目标：O(n*sum) 时间（0/1 背包）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜符号选择转 0/1 背包计数
// - 核心要点：
//   1. 思路起点：设正号元素和为 P、负号元素和为 N，则 P-N=target 且 P+N=total，故 P=(total+target)/2。
//   2. 执行逻辑：1. 检查 |target|<=total 且 total+target 为偶数；2. ways[0]=1；3. 每个数对容量从 targetSum 向下做 ways[s]+=ways[s-value]。
//   3. 为什么这样做：代数变换在符号方案与和为 P 的下标子集间建立一一对应；倒序计数确保每个下标只选择一次，包含 0 时方案数会正确翻倍。
// - 边界与易错点：不能只判断 total<target，负 target 也要看绝对值；奇偶不符直接为 0；0 是不同符号选择，计数不可去重。
// - 举一反三：把正负、两组分配问题化为子集和，是背包建模的重要技巧；先列方程往往比直接设计 DP 更清晰。
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

