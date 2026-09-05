// ============================================================================
// LC-153：寻找旋转排序数组中的最小值
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 4 周 / 第 22 天
// ----------------------------------------------------------------------------
// 题目描述：
// 已知一个长度为 n 的数组，预先按照升序排列，经由 1 到 n 次旋转后，得到输入数组。
// 例如，原数组 nums = [0,1,2,4,5,6,7] 在变化后可能得到：若旋转 4 次，则可以得到 [4,5,6,7,0,1,2] 若旋转 7 次，则可以得到 [0,1,2,4,5,6,7]
// 注意，数组 [a[0], a[1], a[2], ..., a[n-1]] 旋转一次的结果为数组 [a[n-1], a[0], a[1], a[2], ..., a[n-2]]。
// 给你一个元素值互不相同的数组 nums，它原来是一个升序排列的数组，并按上述情形进行了多次旋转。
// 请你找出并返回数组中的最小元素。
// 你必须设计一个时间复杂度为 O(log n) 的算法解决此问题。
//
// 约束与要求：
//   - n == nums.length
//   - 1 <= n <= 5000
//   - -5000 <= nums[i] <= 5000
//   - nums 中的所有整数互不相同
//   - nums 原来是一个升序排序的数组，并进行了 1 至 n 次旋转
//
// 复杂度目标：O(log n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜旋转边界二分：用右端锚点判断 middle 在断点哪一侧
//
// 0. 优化是怎么来的
//   最自然正确的方法是线性扫描所有元素取最小值，O(n)。也可以扫描相邻元素寻找唯一下降点 nums[i]>nums[i+1]，其后一项就是最小值，但最坏仍需 O(n)。
//
//   这里真正可利用的是旋转结构只有一个断点。我们不必逐个寻找下降点，只需判断 middle 在断点左侧还是右侧。为什么选择 nums[right] 作为锚点？当前候选区间的 right 位于包含最小值的那一侧或恰好就是候选末端；元素互异时：
//   - nums[mid] > nums[right]：mid 与 right 之间跨过断点，mid 一定在高值段，最小值严格在 mid 右侧；
//   - nums[mid] < nums[right]：mid 到 right 已经处于同一升序段，最小值不可能在 mid 右侧更深处，mid 自己却可能就是最小值。
//
//   于是一次比较把一半位置排除，得到寻找旋转边界的 O(log n) 二分。
//
// 1. 图像直觉
//   nums = [4,5,6,7,0,1,2]
//
//   4 5 6 7 | 0 1 2
//   高值段      ^ 低值段
//               最小值/断点
//
//   第一轮 middle=3 -> nums[mid]=7，right=2：
//   7 > 2，说明 middle 还在断点左边，
//   最小值只能在 (middle,right]：left=middle+1。
//
//   之后若 middle 落到 0 或 1：
//   nums[mid] < nums[right]，
//   说明断点已经在 [left,middle]，middle 仍可能正是最小值，
//   所以只能 right=middle，不能 middle-1。
//
// 2. 一句话核心
//   把最小值当成旋转断点；用 nums[right] 作锚点判断 middle 在高值段还是低值段，并始终保留可能就是最小值的 middle。
//
// 3. 公式 / 不变量
//   候选不变量：最小值下标始终属于闭区间 [left,right]。
//
//   while left < right:
//       middle = left + (right-left)/2
//       if nums[middle] > nums[right]:
//           # middle 在高值段，不可能是最小值
//           left = middle+1
//       else:
//           # middle 在低值段，可能就是最小值
//           right = middle
//
//   left==right 时只剩唯一候选，返回 nums[left]。
//
//   由于元素互异，middle<right 时不会出现 nums[middle]==nums[right] 的模糊情况。
//
// 4. 执行步骤
//   1. 初始化 left=0、right=n-1，让最小值始终被闭区间包住；这里循环只需 left<right，因为相遇时答案已经唯一
//   2. 计算 middle，并比较 nums[middle] 与当前 nums[right]，不要与固定的 nums[n-1] 混淆候选区间语义
//   3. 若 nums[middle]>nums[right]，证明断点严格在 middle 右侧，令 left=middle+1
//   4. 否则 middle 已在低值段且可能本身就是最小值，令 right=middle 保留它
//   5. left==right 时返回 nums[left]
//   记忆：最小值就是断点；mid>right 去右边，否则保留 mid 去左边。
//
// 5. 为什么不会漏 / 不会重
//   若 nums[middle]>nums[right]，在原始严格升序数组中不可能出现左值大于右值，说明 middle 到 right 之间必跨旋转断点，因此最小值严格位于 middle 右侧，删除到 middle 安全。否则 nums[middle]<nums[right]，middle..right 本身严格升序，这段内部除 middle 外不可能有更小的旋转断点；最小值只能在 [left,middle]，且 middle 可能就是它，所以 right=middle。两种更新都保留最小值并严格缩小区间，最终唯一候选即答案。
//
// 6. 边界与易错点
//   第二分支必须 `right=middle`，不能 `middle-1`，因为 middle 可能正好指向最小值。循环使用 left<right 与该保留策略配套。题目元素互异使与 right 的比较没有歧义；若允许重复，等号时不能判断断点在哪边，通常只能 right--。不要先额外找 pivot 再做别的操作，本题直接把 pivot 当二分目标即可。
//
// 7. 举一反三
//   LC-153 是 LC-34 的‘边界二分’思想在旋转结构上的版本：不是搜索某个具体 target，而是在两个结构区段之间找转折点。LC-154 加入重复值后会暴露信息不足导致的退化；峰值、谷值、第一处违背性质的位置也常能用‘比较相邻/锚点后排除一侧’来构造二分。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最小值。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int findMin(vector<int>& nums) {
        // [left,right] 始终包含全局最小值；left<right 时至少还有两个候选位置。
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left < right) {
            int middle = left + (right - left) / 2;

            // 与右端比较能判断 middle 是否落在旋转前的“大值段”。
            // nums[middle] > nums[right]：断点/最小值一定严格在 middle 右侧，所以丢掉 middle。
            if (nums[middle] > nums[right]) left = middle + 1;
            // 否则 middle 位于包含最小值的“小值段”，最小值可能就是 middle，因此保留 middle。
            else right = middle;
        }
        // 区间收缩到一个位置时，该位置就是旋转断点，也就是全局最小值。
        return nums[left];
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
    cout << sol.findMin(a) << "\n";
    return 0;
}
