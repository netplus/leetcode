// ============================================================================
// LC-152：乘积最大子数组
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 3 周 / 第 21 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums，请你找出数组中乘积最大的非空连续子数组 （该子数组中至少包含一个数字），并返回该子数组所对应的乘积。
// 测试用例的答案是一个 32-位整数。
// 请注意，一个只包含一个元素的数组的乘积是这个元素的值。
//
// 约束与要求：
//   - 1 <= nums.length <= 2 * 10^4
//   - -10 <= nums[i] <= 10
//   - nums 的任何子数组的乘积都保证是一个 32-位整数
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜双状态 DP：同时保留最大与最小乘积后缀
// - 核心要点：
//   1. 思路起点：负数会交换最大和最小的角色，因此对每个位置必须同时保存“以此结尾”的最大、最小乘积。
//   2. 执行逻辑：1. 用首元素初始化 currentMax/currentMin/best；2. 遇负数先交换最大最小；3. 分别在单独从 x 开始与延续旧后缀间取 max/min，再更新 best。
//   3. 为什么这样做：所有以当前位置结尾的子数组只有 x、旧最大*x、旧最小*x 三类候选；负数交换后按 max/min 更新恰覆盖三者。
// - 边界与易错点：不能只维护最大值；0 会自然让状态从 0 或后续元素重启；全负或单元素时 best 不能初始化为 0。
// - 举一反三：当运算不保持单调性时要保留多个极值状态，例如带负权乘积、绝对值和符号状态 DP。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最大乘积。
// test.in 的预期输出：6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int maxProduct(vector<int>& nums) {
        int currentMax = nums[0];
        int currentMin = nums[0];
        int best = nums[0];
        for (int i = 1; i < static_cast<int>(nums.size()); ++i) {
            const int value = nums[i];
            if (value < 0) swap(currentMax, currentMin);
            currentMax = max(value, currentMax * value);
            currentMin = min(value, currentMin * value);
            best = max(best, currentMax);
        }
        return best;
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
    cout << sol.maxProduct(a) << "\n";
    return 0;
}

