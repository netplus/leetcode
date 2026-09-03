// ============================================================================
// LC-215：数组中的第 K 个最大元素
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 25 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定整数数组 nums 和整数 k，请返回数组中第 k 个最大的元素。
// 请注意，你需要找的是数组排序后的第 k 个最大的元素，而不是第 k 个不同的元素。
// 你必须设计并实现时间复杂度为 O(n) 的算法解决此问题。
//
// 约束与要求：
//   - 1 <= k <= nums.length <= 10^5
//   - -10^4 <= nums[i] <= 10^4
//
// 复杂度目标：O(n) 平均 (快速选择) 或 O(n log k)（heap）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜选择算法：nth_element 定位第 k 大
// - 核心要点：
//   1. 思路起点：第 k 大在升序下标 n-k；nth_element 以平均 O(n) 的选择算法把该位置放成排序后应有的元素，无需完整排序。
//   2. 执行逻辑：1. 计算 targetIndex=n-k；2. 调用 nth_element 使目标位置就位；3. 返回 nums[targetIndex]。
//   3. 为什么这样做：算法结束后目标左侧元素不大于它、右侧不小于它，目标值与完整排序同位置一致，因此就是第 k 大（重复值按出现次数计）。
// - 边界与易错点：题目问排序序列中的第 k 个而非第 k 个不同值；nth_element 会修改输入；若面试要求手写可实现随机 Quickselect。
// - 举一反三：只需顺序统计量而非完整有序时，应优先选择 Quickselect；流式或数据量远大于 k 时用大小为 k 的堆。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n k。
//   第 2 行：n 个以空格分隔的整数。
//   输出：第 k 大元素。
// test.in 的预期输出：5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int findKthLargest(vector<int>& nums, int k) {
        const int targetIndex = static_cast<int>(nums.size()) - k;
        nth_element(nums.begin(), nums.begin() + targetIndex, nums.end());
        return nums[targetIndex];
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
    cout << sol.findKthLargest(a, k) << "\n";
    return 0;
}

