// ============================================================================
// LC-704：二分查找
// 难度：简单
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 22 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个 n 个元素有序的（升序）整型数组 nums 和一个目标值 target，写一个函数搜索 nums 中的 target，如果 target 存在返回下标，否则返回 -1。
// 你必须编写一个具有 O(log n) 时间复杂度的算法。
//
// 约束与要求：
//   - 你可以假设 nums 中的所有元素是不重复的。
//   - n 将在 [1, 10000] 之间。
//   - nums 的每个元素都将在 [-9999, 9999] 之间。
//
// 复杂度目标：O(log n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜标准闭区间二分查找
// - 核心要点：
//   1. 思路起点：维护目标若存在必位于 [left,right] 的不变量；比较中点后可排除包含中点的一半区间。
//   2. 执行逻辑：1. 初始化 left=0、right=n-1；2. 用 left+(right-left)/2 取中点；3. 相等返回；偏小令 left=mid+1，偏大令 right=mid-1。
//   3. 为什么这样做：有序性保证 nums[mid]<target 时 mid 及其左侧都不可能是答案，另一分支对称；区间为空时说明目标不存在。
// - 边界与易错点：闭区间循环条件是 left<=right；更新必须越过 mid 避免死循环；中点写法避免 left+right 溢出。
// - 举一反三：二分的本质是在单调谓词上找边界；掌握区间定义后可扩展到左右边界和答案二分。
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
            const int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;
            if (nums[middle] < target) left = middle + 1;
            else right = middle - 1;
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

