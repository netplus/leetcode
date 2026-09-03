// ============================================================================
// LC-167：两数之和 II - 输入有序数组
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 2 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个下标从 1 开始的整数数组 numbers，该数组已按非递减顺序排列，请你从数组中找出满足相加之和等于目标数 target 的两个数。
// 如果设这两个数分别是 numbers[index_1] 和 numbers[index_2]，则 1 <= index_1 < index_2 <= numbers.length。
// 以长度为 2 的整数数组 [index_1, index_2] 的形式返回这两个整数的下标 index_1 和 index_2。
// 你可以假设每个输入只对应唯一的答案，而且你不可以重复使用相同的元素。
// 你所设计的解决方案必须只使用常量级的额外空间。
//
// 约束与要求：
//   - 2 <= numbers.length <= 3 * 10^4
//   - -1000 <= numbers[i] <= 1000
//   - numbers 按非递减顺序排列
//   - -1000 <= target <= 1000
//   - 仅存在一个有效答案
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜有序数组的相向双指针
// - 核心要点：
//   1. 思路起点：数组有序，左右端点之和相对目标的大小能唯一决定应排除哪一端，而无需哈希表。
//   2. 执行逻辑：1. left/right 指向两端；2. 和偏小就增大 left，和偏大就减小 right；3. 相等时返回 1-based 下标。
//   3. 为什么这样做：若当前和偏小，固定 left 与任何更靠左/不大于 right 的元素都不可能达到目标，所以 left 可安全排除；偏大时对 right 对称成立。
// - 边界与易错点：返回值是 1-based；不能复用同一位置，所以循环条件是 left<right；题目明确要求常量额外空间。
// - 举一反三：排序后两数之和、三数之和的内层、盛水容器和回文检查都依赖“单调性允许排除一端”。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的整数。
//   输出：两个从 1 开始计数的下标，以空格分隔并升序排列。
// test.in 的预期输出：1 2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> twoSum(vector<int>& numbers, int target) {
        int left = 0, right = static_cast<int>(numbers.size()) - 1;
        while (left < right) {
            const long long sum = static_cast<long long>(numbers[left]) + numbers[right];
            if (sum == target) return {left + 1, right + 1};
            if (sum < target) ++left;   // 需要更大的和
            else --right;               // 需要更小的和
        }
        return {};
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target;
    if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.twoSum(a, target);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

