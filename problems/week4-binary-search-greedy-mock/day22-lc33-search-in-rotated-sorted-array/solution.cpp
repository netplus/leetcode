// ============================================================================
// LC-33：搜索旋转排序数组
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 22 天
// ----------------------------------------------------------------------------
// 题目描述：
// 整数数组 nums 按升序排列，数组中的值互不相同。
// 在传递给函数之前，nums 在预先未知的某个下标 k （0 <= k < nums.length）上进行了向左旋转，使数组变为 [nums[k], nums[k+1], ..., nums[n-1],
// nums[0], nums[1], ..., nums[k-1]] （下标从 0 开始计数）。
// 例如，[0,1,2,4,5,6,7] 下标 3 上向左旋转后可能变为 [4,5,6,7,0,1,2]。
// 给你旋转后的数组 nums 和一个整数 target，如果 nums 中存在这个目标值 target，则返回它的下标，否则返回 -1。
// 你必须设计一个时间复杂度为 O(log n) 的算法解决此问题。
//
// 约束与要求：
//   - 1 <= nums.length <= 5000
//   - -10^4 <= nums[i] <= 10^4
//   - nums 中的每个值都独一无二
//   - 题目数据保证 nums 在预先未知的某个下标上进行了旋转
//   - -10^4 <= target <= 10^4
//
// 复杂度目标：O(log n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜旋转有序数组二分：每轮至少一半有序
// - 核心要点：
//   1. 思路起点：数组值互异；任取中点，左半或右半必有一侧保持升序。先识别有序侧，再判断 target 是否落在其值域内。
//   2. 执行逻辑：1. 比较 nums[left] 与 nums[mid] 判定左侧是否有序；2. target 在有序侧闭开值域内就保留该侧；3. 否则搜索另一侧。
//   3. 为什么这样做：有序侧的端点值可准确判断目标是否位于其中；若不在即可整体排除。每轮都保留包含目标的半区，最终命中或区间为空。
// - 边界与易错点：值互异是直接判定有序侧的关键；值域端点一边含等号、一边不含，避免重复保留 mid。
// - 举一反三：对局部有序结构做二分时，先找可验证单调的一段；有重复值时往往需退化地缩边界。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的整数。
//   输出：下标；未找到时输出 -1。
// test.in 的预期输出：4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;

            if (nums[left] <= nums[middle]) {  // 左半段有序
                if (nums[left] <= target && target < nums[middle]) right = middle - 1;
                else left = middle + 1;
            } else {                           // 右半段有序
                if (nums[middle] < target && target <= nums[right]) left = middle + 1;
                else right = middle - 1;
            }
        }
        return -1;
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
    cout << sol.search(a, target) << "\n";
    return 0;
}

