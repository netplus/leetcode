// ============================================================================
// LC-34：在排序数组中查找元素的第一个和最后一个位置
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 22 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个按照非递减顺序排列的整数数组 nums，和一个目标值 target。
// 请你找出给定目标值在数组中的开始位置和结束位置。
// 如果数组中不存在目标值 target，返回 [-1, -1]。
// 你必须设计并实现时间复杂度为 O(log n) 的算法解决此问题。
//
// 约束与要求：
//   - 0 <= nums.length <= 10^5
//   - -10^9 <= nums[i] <= 10^9
//   - nums 是一个非递减数组
//   - -10^9 <= target <= 10^9
//
// 复杂度目标：O(log n) 时间（两次二分查找）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜边界二分：两次 lower_bound
// - 核心要点：
//   1. 思路起点：把“找任意等于 target”改成找第一个 >=target 的位置；右边界可由第一个 >=target+1（即 >target）的位置减一得到。
//   2. 执行逻辑：1. 实现半开区间 [left,right) 的 firstAtLeast；2. 求 firstAtLeast(target) 并验证是否命中；3. 求 firstAtLeast(target+1)-1 作为末位置。
//   3. 为什么这样做：单调谓词 nums[i]>=value 在边界前假、边界后真，二分返回首个真位置；所有 target 恰位于两个边界之间。
// - 边界与易错点：target+1 可能溢出，接口用 long long value；半开区间循环 left<right；空数组和未命中返回 [-1,-1]。
// - 举一反三：计数等于 lower_bound(>x)-lower_bound(>=x)；数据库范围查询与有序数组插入点同样是边界二分。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的整数。
//   输出：边界 "left right"；未找到时输出 "-1 -1"。
// test.in 的预期输出：3 4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    static int firstAtLeast(const vector<int>& nums, long long value) {
        int left = 0, right = static_cast<int>(nums.size());
        while (left < right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] < value) left = middle + 1;
            else right = middle;
        }
        return left;
    }

public:
    vector<int> searchRange(vector<int>& nums, int target) {
        int first = firstAtLeast(nums, target);
        if (first == static_cast<int>(nums.size()) || nums[first] != target) return {-1, -1};
        int afterLast = firstAtLeast(nums, static_cast<long long>(target) + 1);
        return {first, afterLast - 1};
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.searchRange(a, target);
    cout << ans[0] << " " << ans[1] << "\n";
    return 0;
}

