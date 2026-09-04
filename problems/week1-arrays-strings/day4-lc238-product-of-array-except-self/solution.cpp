// ============================================================================
// LC-238：除自身以外数组的乘积
// 难度：中等
// 优先级：P1（进阶）
// 学习进度：第 1 周 / 第 4 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums，返回数组 answer，其中 answer[i] 等于 nums 中除了 nums[i] 之外其余各元素的乘积。
// 题目数据保证数组 nums 之中任意元素的全部前缀元素和后缀的乘积都在 32 位整数范围内。
// 请不要使用除法，且在 O(n) 时间复杂度内完成此题。
//
// 约束与要求：
//   - 2 <= nums.length <= 10^5
//   - -30 <= nums[i] <= 30
//   - 输入保证数组 answer[i] 在 32 位整数范围内
//
// 复杂度目标：O(n) 时间，O(1) 额外空间（不计输出结果）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜左右乘积：复用输出数组
//
// 0. 优化是怎么来的
//   最直接的做法，是对每个 i 再遍历一遍数组，把所有 j!=i 的值相乘；这样每个答案都重新计算大量相同乘积，总时间 O(n^2)。
//
//   对固定 i，真正需要的信息天然只有两块：i 左边所有元素的乘积，以及 i 右边所有元素的乘积。相邻位置之间，这两块累计值可以持续复用：从左扫描时 prefix 只需乘上刚经过的一个元素；从右扫描时 suffix 同理。
//
//   于是把“每个 i 重新乘 n 个元素”改成“两次线性扫描维护可延续累计状态”。最终 answer[i]=leftProduct*rightProduct。
//
//   优化类型：重复计算 -> 前缀/后缀累计状态复用。
//
// 1. 图像直觉
//   对位置 i 来说，“除了自己以外”天然分成左右两块：
//
//   nums = [1, 2, 3, 4]
//                 ^ i=2
//
//   左边乘积：1 * 2 = 2
//   右边乘积：4     = 4
//   answer[2] = 2 * 4 = 8
//
//   不需要算全体乘积再除 nums[i]。
//   第一趟从左往右，把“来到 i 之前已经乘过的东西”写进 answer[i]；
//   第二趟从右往左，再把“来到 i 之前从右侧已经乘过的东西”乘进去。
//
//   关键点是：两趟都必须先使用累计值，再把 nums[i] 纳入累计，这样 nums[i] 自己才不会被乘进去。
//
// 2. 一句话核心
//   把除自身乘积分解成“左侧所有元素乘积 × 右侧所有元素乘积”，分别用两趟扫描维护。
//
// 3. 公式 / 不变量
//   answer[i]
//   = product(nums[0..i-1]) * product(nums[i+1..n-1])
//
//   第一趟：
//   answer[i] = leftProduct
//   leftProduct *= nums[i]
//
//   第二趟：
//   answer[i] *= rightProduct
//   rightProduct *= nums[i]
//
// 4. 执行步骤
//   1. answer 初始化为 1，leftProduct=1
//   2. 左到右：先把 leftProduct 写给 answer[i]，再执行 leftProduct*=nums[i]
//   3. rightProduct=1，从右到左扫描
//   4. 先执行 answer[i]*=rightProduct，再执行 rightProduct*=nums[i]
//   5. 最终每个 answer[i] 同时拥有 i 左侧和右侧的乘积
//   记忆：先用累计值回答当前位置，再把当前位置纳入累计。左右各做一遍。
//
// 5. 为什么不会漏 / 不会重
//   第一趟到达 i 时，leftProduct 只包含 i 左边元素；第二趟到达 i 时，rightProduct 只包含 i 右边元素。
//   两者相乘覆盖所有其他元素且恰好排除 nums[i]。
//
// 6. 边界与易错点
//   两趟的更新顺序都不能反：若先乘 nums[i] 再写/使用累计值，就会把自己算进去；0 不需要额外分支，这种左右分解天然处理一个或多个 0；
//   空间复杂度通常不计返回数组 answer。
//
// 7. 举一反三
//   前后缀分解不只用于乘积：只要某个位置的答案能拆成“左侧聚合 ×/+/max/... 右侧聚合”，就可以考虑左右两趟扫描。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：n 个以空格分隔的乘积结果。
// test.in 的预期输出：24 12 8 6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> productExceptSelf(vector<int>& nums) {
        vector<int> answer(nums.size(), 1);
        // 实现技巧（滚动暂存）：answer[i] 先暂存"i 之前所有前缀元素的乘积"，
        // 写完后把 nums[i] 乘进 leftProduct，这份更新后的暂存值就是下一轮
        // answer[i+1] 需要的前缀乘积——暂存值就地滚动，不需要额外数组。
        int leftProduct = 1;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            answer[i] = leftProduct;
            leftProduct *= nums[i];
        }

        // 右往左同理：rightProduct 暂存右侧后缀乘积，先乘入 answer[i]，
        // 再把 nums[i] 纳入，滚动成下一轮的后缀乘积。
        int rightProduct = 1;
        for (int i = static_cast<int>(nums.size()) - 1; i >= 0; --i) {
            answer[i] *= rightProduct;
            rightProduct *= nums[i];
        }
        return answer;
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
    auto ans = sol.productExceptSelf(a);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}
