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
// 解法精讲｜边界二分：从找一个 target 升级为找单调谓词第一次成立的位置
//
// 0. 优化是怎么来的
//   一个自然正确做法是线性扫描数组，记录第一次和最后一次遇到 target 的位置，时间 O(n)。即使先用普通二分 O(log n) 找到某个 target，再向左右扩展，极端输入全部都是 target 时扩展仍会退化到 O(n)。
//
//   浪费在于：我们已经知道数组非递减，却还在边界附近逐个确认重复值。对于固定 value，谓词 P(i)=[nums[i]>=value] 具有单调性：一旦某位置为真，右边所有位置都为真；一旦某位置为假，左边所有位置都为假。
//
//   所以问题不再是‘某个位置等不等于 target’，而是‘假区间和真区间的分界点在哪里’。二分每次可以整段排除仍为假的左半或已经为真的右半，直接找到 first true。用 value=target 找到 target 段开始，用 value=target+1 找到第一个严格大于 target 的位置，再减一得到结束位置。
//
// 1. 图像直觉
//   nums = [5,7,7,8,8,10], target=8
//
//   对 P(i): nums[i] >= 8：
//   index: 0 1 2 | 3 4 5
//   P:     F F F | T T T
//                 ^ firstAtLeast(8)=3
//
//   对 nums[i] >= 9：
//   index: 0 1 2 3 4 | 5
//   P:     F F F F F | T
//                     ^ firstAtLeast(9)=5
//
//   所以所有 8 恰好位于 [3, 5-1] = [3,4]。
//
//   边界二分不是‘找到 8 后继续找’，而是直接搜索 F/T 的分界线。
//
// 2. 一句话核心
//   把数值比较改写成单调真假谓词，二分寻找第一个满足 nums[i]>=value 的位置；两个分界点夹住全部 target。
//
// 3. 公式 / 不变量
//   firstAtLeast(value)：寻找最小 i，使 nums[i] >= value；若不存在，返回 n。
//
//   使用半开区间 [left,right)：
//   初始 [0,n)
//   while left < right:
//       middle = left + (right-left)/2
//       nums[middle] < value -> middle 仍是假，left = middle+1
//       nums[middle] >= value -> middle 已是真且可能是第一个真，right = middle
//   结束时 left==right，即第一个真位置。
//
//   first = firstAtLeast(target)
//   afterLast = firstAtLeast((long long)target + 1)
//   答案 = [first, afterLast-1]，前提是 first<n 且 nums[first]==target。
//
// 4. 执行步骤
//   1. 先实现唯一的 firstAtLeast(nums,value)，明确它在半开区间 [left,right) 中寻找第一个真位置
//   2. 求 first=firstAtLeast(target)；若 first==n 或 nums[first]!=target，说明 target 根本不存在，返回 [-1,-1]
//   3. 求 afterLast=firstAtLeast((long long)target+1)，它是第一个严格大于 target 的位置
//   4. 返回 [first,afterLast-1]；两个边界搜索都保持 O(log n)
//   记忆：找重复值边界，不要先命中再扩展；直接二分 F...F | T...T 的分界。
//
// 5. 为什么不会漏 / 不会重
//   firstAtLeast 中，若 nums[middle]<value，则 middle 及其左侧都不可能满足谓词，所以 left=middle+1 安全；否则 middle 已满足谓词，但可能正是最左边那个，故只能令 right=middle 保留它。循环结束时假区间已全部在 left 左侧、真区间从 left 开始。对 target 与 target+1 分别求界后，位于两界之间的值既 >=target 又 <target+1；对整数即恰等于 target，因此不会漏也不会多。
//
// 6. 边界与易错点
//   这里刻意使用半开区间 [left,right)，循环是 left<right，命中谓词时 right=middle 而不是 middle-1，因为 middle 可能就是答案。target 可能是 INT_MAX，所以不能直接用 int 的 target+1；当前实现把 value 提升为 long long。先验证 first 真正命中 target，再计算结果可正确处理空数组和不存在情况。
//
// 7. 举一反三
//   这是比 LC-704 更通用的二分形态：lower_bound、upper_bound、插入位置、计数区间都可视为找单调谓词的边界。Day 23 的‘答案二分’只是把下标轴换成答案值轴：仍然是寻找 false...false|true...true 的第一次成立位置。
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
        // 搜索半开区间 [left,right)，函数契约是返回第一个 nums[i]>=value 的位置；
        // 若所有元素都更小，则返回 nums.size() 这个合法的“尾后位置”。
        int left = 0, right = static_cast<int>(nums.size());
        while (left < right) {
            int middle = left + (right - left) / 2;
            // nums[middle] < value 时，middle 不可能是答案，连同左侧一起丢弃；
            // 否则 middle 仍可能就是第一个满足位置，所以 right=middle 而不是 middle-1。
            if (nums[middle] < value) left = middle + 1;
            else right = middle;
        }
        return left;
    }

public:
    vector<int> searchRange(vector<int>& nums, int target) {
        // first 是第一个 >=target 的位置；只有它真实等于 target 时，目标才存在。
        int first = firstAtLeast(nums, target);
        if (first == static_cast<int>(nums.size()) || nums[first] != target) return {-1, -1};

        // 第一个 >=target+1 的位置，就是所有 target 的尾后边界；减一得到最后一个 target。
        // 用 long long 做 target+1，避免 target==INT_MAX 时整数溢出。
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
