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
// 题目示例：
// 示例 1：
//   Input: nums = [-1,0,3,5,9,12], target = 9
//   Output: 4
//   Explanation: 9 exists in nums and its index is 4
//
// 示例 2：
//   Input: nums = [-1,0,3,5,9,12], target = 2
//   Output: -1
//   Explanation: 2 does not exist in nums so return -1
//
// 约束与要求：
//   - 1 <= nums.length <= 10^4
//   - -10^4 < nums[i], target < 10^4
//   - nums 中的所有整数互不相同
//   - nums 按严格升序排列
//
// 复杂度目标：O(log n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜标准二分：把一次比较升级成整段候选淘汰
//
// 0. 优化是怎么来的
//   最自然且正确的算法是从左到右逐个比较 nums[i] 与 target；找到就返回 i，否则扫描结束返回 -1。它不会漏，因为每个元素都检查了，但最坏需要 O(n) 次比较。
//
//   真正可以省掉的工作来自数组已经严格升序。若 nums[mid] < target，那么 left..mid 中每个值都 <= nums[mid] < target，这整段无需再逐个检查；若 nums[mid] > target，mid..right 同理可以整体淘汰。
//
//   因此一次比较不再只排除一个元素，而是排除至少约一半候选。只要维护‘答案若存在一定仍在当前候选区间’这个不变量，每轮把不可能的一半永久丢掉，候选规模 n -> n/2 -> n/4 -> ...，最终就是 O(log n) 的二分查找。
//
// 1. 图像直觉
//   nums = [-1,0,3,5,9,12], target = 9
//
//   left=0                right=5
//   [-1, 0, 3, 5, 9, 12]
//            ^ mid=2, nums[mid]=3
//
//   3 < 9，而且数组升序：
//   [-1, 0, 3] 这一整段都不可能再出现 9。
//                x x x
//
//   候选直接缩成 [3,5]，而不是只把下标 2 划掉。
//
//   二分的核心图像就是：
//   一次中点比较 -> 给一整段候选判死刑。
//
// 2. 一句话核心
//   始终让 [left,right] 包住所有仍可能等于 target 的位置；比较中点后，利用有序性一次淘汰不可能的一半。
//
// 3. 公式 / 不变量
//   闭区间不变量：
//   target 若存在，则其下标一定属于 [left,right]。
//
//   middle = left + (right-left)/2
//
//   nums[middle] == target -> 命中
//   nums[middle] <  target -> [left,middle] 全部过小，left = middle+1
//   nums[middle] >  target -> [middle,right] 全部过大，right = middle-1
//
//   循环条件 left <= right，因为 left==right 时仍有一个真实候选没有检查。
//   结束时 left > right，候选集合为空。
//
// 4. 执行步骤
//   1. 初始化 left=0、right=n-1，把整个数组定义为闭区间候选集合
//   2. 只在 left<=right 时循环，并用 left+(right-left)/2 计算 middle
//   3. 若 nums[middle]==target 立即返回 middle
//   4. 若中点值过小，把 left 更新为 middle+1；若过大，把 right 更新为 middle-1，更新必须越过已判定的 middle
//   5. 区间变空仍未命中时返回 -1
//   记忆：闭区间包住全部候选；中点比较不是排除一个点，而是证明一整段不可能。
//
// 5. 为什么不会漏 / 不会重
//   若中点值小于 target，由严格升序可知所有 i<=middle 都满足 nums[i]<=nums[middle]<target，因此删除 [left,middle] 不会删掉答案；中点值大于 target 时对称。每次更新都只删除已被有序性证明不可能的元素，所以不会漏。middle 命中时立即结束；未命中时 middle 被永久移出候选区间，因此不会重复检查同一候选，区间最终必然缩空或命中。
//
// 6. 边界与易错点
//   先确定区间语义再写循环。闭区间对应 left<=right、right=n-1、更新 middle±1；不要把半开区间模板混进来。middle 用 left+(right-left)/2 避免 left+right 的潜在整数溢出。题目严格升序且元素唯一，所以命中任意一个就是唯一答案。
//
// 7. 举一反三
//   LC-34 会把‘找某个值’升级成‘找真假分界线’；LC-33/153 会说明即使整体不再有序，只要一次比较仍能证明一批候选不可能，二分思想仍可成立。之后遇到二分题，先问的不是‘mid 怎么写’，而是‘一次判断能否安全淘汰整段候选’。
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
        // 搜索区间采用闭区间 [left,right]；只要 left<=right，就仍至少有一个候选位置。
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            // 这样写与 (left+right)/2 等价，但避免两个大下标直接相加可能溢出。
            const int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;

            // 数组严格递增：middle 值偏小时，middle 及其左侧都不可能等于 target；
            // 反之 middle 及其右侧都可整体排除。更新后仍保持“若答案存在，它一定在 [left,right]”。
            if (nums[middle] < target) left = middle + 1;
            else right = middle - 1;
        }
        // left>right 表示候选闭区间已经为空。
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
