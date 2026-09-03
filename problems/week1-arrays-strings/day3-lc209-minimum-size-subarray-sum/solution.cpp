// ============================================================================
// LC-209：长度最小的子数组
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 3 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个含有 n 个正整数的数组和一个正整数 target。
// 找出该数组中满足其总和大于等于 target 的长度最小的子数组 [nums_l, nums_l+1, ..., nums_r-1, nums_r]，并返回其长度。
// 如果不存在符合条件的子数组，返回 0。
//
// 约束与要求：
//   - 1 <= target <= 10^9
//   - 1 <= nums.length <= 10^5
//   - 1 <= nums[i] <= 10^4
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜正数数组上的可变滑动窗口
//
// 1. 图像直觉
//   例：nums = [2, 3, 1, 2, 4, 3], target = 7。
//
//   right 一直向右扩张：
//   [2, 3, 1, 2]  sum = 8  已经 >= 7
//    L        R
//
//   既然已经够了，就尝试从左边“挤掉”元素，让窗口更短：
//   去掉 2 后：
//      [3, 1, 2]  sum = 6  又不够了
//       L     R
//
//   因为数组里全是正数，右边加元素只会让 sum 变大，左边删元素只会让 sum 变小。
//   所以窗口一旦“够了”就放心缩左边；缩到“不够”的那一刻，再继续扩右边。
//
// 2. 一句话核心
//   右边负责把窗口扩到够用，左边负责在仍然够用时尽可能压缩；正数保证这两个方向都不需要回头。
//
// 3. 公式 / 不变量
//   windowSum = nums[left] + ... + nums[right]
//
//   所有 nums[i] > 0，因此：
//   right++  => windowSum 只会增大
//   left++   => windowSum 只会减小
//
//   当 windowSum >= target 时：
//   先记录当前长度，再不断 left++，直到 windowSum < target。
//
// 4. 执行步骤
//   1. left=0，sum=0，best 设为 n+1 哨兵
//   2. right 每前进一步，把 nums[right] 加入 sum
//   3. 只要 sum>=target，就先更新 best，再减去 nums[left] 并 left++
//   4. 收缩到 sum<target 后停止，继续扩张 right
//   5. 扫描结束，best 未更新则返回 0
//   记忆：右边扩到够 -> 左边缩到刚好不够 -> 再扩右边。
//
// 5. 为什么不会漏 / 不会重
//   固定一个 right 时，随着 left 右移，正数保证窗口和只会下降。因此 while 循环会从较长合法窗口一路缩到最短合法窗口，
//   再多缩一步就非法；这个 right 下不可能还有更短合法起点被漏掉。left/right 都只向右移动，所以总复杂度 O(n)。
//
// 6. 边界与易错点
//   正数条件是整个算法成立的根基；若含负数，删掉左端后 sum 可能反而变大，窗口就失去单调性。
//   更新 best 必须发生在移除左端之前；sum 用 long long 更稳妥。
//
// 7. 举一反三
//   判断能否用滑动窗口时，不要先看题目有没有“子数组”，而要先问：扩右、缩左时目标状态是否具有单调性。
//   含负数的最短子数组和需要换前缀和/单调队列等方法。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最短长度。
// test.in 的预期输出：2
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int minSubArrayLen(int target, vector<int>& nums) {
        int left = 0;
        int best = static_cast<int>(nums.size()) + 1;
        long long sum = 0;
        for (int right = 0; right < static_cast<int>(nums.size()); ++right) {
            sum += nums[right];
            while (sum >= target) {
                best = min(best, right - left + 1);
                sum -= nums[left++];
            }
        }
        return best == static_cast<int>(nums.size()) + 1 ? 0 : best;
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
    cout << sol.minSubArrayLen(k, a) << "\n";
    return 0;
}
