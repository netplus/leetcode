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
// - 核心要点：
//   1. 思路起点：answer[i] 先保存 i 左侧所有元素的乘积，再从右向左乘上 i 右侧乘积；全程不需要除法。
//   2. 执行逻辑：1. 左到右写入左侧乘积并更新 prefix；2. rightProduct 从 1 开始右到左扫描；3. 把右侧乘积乘入答案后再纳入 nums[i]。
//   3. 为什么这样做：第一次扫描后 answer[i]=乘积(0..i-1)；第二次到达 i 时 rightProduct=乘积(i+1..n-1)，两者相乘正好排除 nums[i]。
// - 边界与易错点：更新 rightProduct 的顺序不能提前；0 的个数无需特殊判断；额外空间统计通常不含返回数组。
// - 举一反三：前后缀分解适用于除自身聚合、左右最大值、分割点条件和树上的换根 DP。
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
        int leftProduct = 1;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            answer[i] = leftProduct;
            leftProduct *= nums[i];
        }

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

