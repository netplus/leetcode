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
//
// 0. 优化是怎么来的
//   最直接的做法仍然是枚举所有下标对 (i,j)，检查 numbers[i]+numbers[j] 是否等于 target，时间 O(n^2)。
//
//   题目额外给出的“数组有序”意味着一次比较不只排除当前这一对。当两端和太小时，left 已经与当前最大的 right 相加都不够，那么 left 与中间任何更小元素相加都不可能成功；这一整批候选可以同时删除。和太大时对 right 完全对称。
//
//   优化类型：存在单调性 -> 一次判断排除整批候选。于是两层枚举被压缩成两根只向内移动的指针，总扫描 O(n)。
//
// 1. 图像直觉
//   例：numbers = [2, 7, 11, 15], target = 9。
//
//   L                              R
//   2        7        11          15
//   |                              |
//   +------------- 17 -------------+
//
//   17 > 9。关键不是“随便把右指针左移”，而是：
//   15 已经和当前最小候选 2 相加都太大了；那么 15 和 7、11 相加只会更大。
//   所以 right=15 这一整列候选都可以一次性排除。
//
//   如果当前和 < target，则完全对称：left 已经和当前最大候选 right 相加都太小，left 这一整行都不可能再成功。
//
// 2. 一句话核心
//   每次比较两端之和，利用有序性证明其中一端已经不可能参与任何剩余答案，然后安全丢掉这一端。
//
// 3. 公式 / 不变量
//   sum = numbers[left] + numbers[right]
//
//   sum < target:
//     对任意 j <= right，numbers[left] + numbers[j] <= sum < target
//     => left 不可能参与答案，left++
//
//   sum > target:
//     对任意 i >= left，numbers[i] + numbers[right] >= sum > target
//     => right 不可能参与答案，right--
//
// 4. 执行步骤
//   1. left 指向最小候选，right 指向最大候选
//   2. 计算 sum=numbers[left]+numbers[right]
//   3. sum<target：排除 left；sum>target：排除 right
//   4. sum==target：返回题目要求的 1-based 下标
//   记忆：和小 -> 左端不可能，left++；和大 -> 右端不可能，right--。
//
// 5. 为什么不会漏 / 不会重
//   每次移动指针前，都能用有序性证明被丢弃的端点不可能和当前候选区间中的任何元素组成 target，因此不会漏解；
//   每一步至少丢弃一个端点，所以最多线性扫描一次。
//
// 6. 边界与易错点
//   返回的是 1-based 下标；循环必须是 left<right，不能复用同一位置；如果题目没有“有序”这个前提，就不能用上述整行/整列排除逻辑，应回到哈希表等方法。
//
// 7. 举一反三
//   LC-15 会直接复用这个模型：先固定一个数，把三数之和降维成“在剩余有序区间里找两数之和”。双指针的本质不是两个变量，而是单调性允许一次排除一批候选。
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
            if (sum < target) ++left;   // 当前 left 连最大 right 都凑不够，可安全排除
            else --right;               // 当前 right 连最小 left 都超标，可安全排除
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
