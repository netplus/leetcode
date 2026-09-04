// ============================================================================
// LC-15：三数之和
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 2 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums，判断是否存在三元组 [nums[i], nums[j], nums[k]] 满足 i != j、i != k 且 j != k，同时还满足 nums[i] + nums[j] +
// nums[k] == 0。
// 请你返回所有和为 0 且不重复的三元组。
// 注意：答案中不可以包含重复的三元组。
//
// 约束与要求：
//   - 3 <= nums.length <= 3000
//   - -10^5 <= nums[i] <= 10^5
//
// 复杂度目标：O(n^2) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜排序 + 枚举锚点 + 双指针去重
//
// 0. 优化是怎么来的
//   最自然的 3Sum 是三层循环枚举 i、j、k，时间 O(n^3)。真正的突破不是直接记住“三数之和要排序双指针”，而是先固定一个数 nums[i]：剩余问题立即变成“在后面的元素里寻找两数之和等于 -nums[i]”。
//
//   如果剩余区间无序，这个二数问题仍要反复搜索；排序之后，就可以完整复用 LC-167 的单调淘汰：和小排除 left，和大排除 right。这样每个固定 i 只需线性扫描一次。
//
//   优化类型：问题降维 + 排序制造单调性。复杂度从 O(n^3) 降为排序 O(n log n) 加 n 次 O(n) 双指针，即 O(n^2)。
//
// 1. 图像直觉
//   排序后，不要把 3Sum 看成一个全新的问题，而是先固定一个锚点：
//
//   nums = [-4, -1, -1, 0, 1, 2]
//                i   L        R
//               -1  -1        2
//
//   固定 nums[i] = -1 后：
//   (-1) + nums[L] + nums[R] = 0
//           ↓
//   nums[L] + nums[R] = 1
//
//   于是剩余部分就是一题 LC-167：在有序区间里找“两数之和 = -nums[i]”。
//   找到 [-1,-1,2] 后，L/R 要跨过相同值；下一个 i 如果仍是 -1，也要直接跳过，否则会重复输出同一个值三元组。
//
// 2. 一句话核心
//   先排序并固定第一个数，把 3Sum 降维成很多次有序 Two Sum；去重发生在“锚点”和“命中后的左右值”两个层次。
//
// 3. 公式 / 不变量
//   nums[i] + nums[left] + nums[right] = 0
//
//   固定 nums[i] 后：
//   nums[left] + nums[right] = -nums[i]
//
//   这正是 LC-167 的双指针子问题。
//
// 4. 执行步骤
//   1. 先排序，让剩余两数具有可利用的单调性
//   2. 枚举锚点 i；若 nums[i] 与前一个锚点相同则跳过
//   3. 在 [i+1,n-1] 上运行相向双指针，目标为 -nums[i]
//   4. 命中后记录三元组，并让 left/right 分别跨过当前重复值
//   5. 若 nums[i]>0，可直接结束：后面三个数都不会再凑出 0
//   记忆：排序 -> 固定一个 -> 剩下变 Two Sum -> 命中后跳重复。
//
// 5. 为什么不会漏 / 不会重
//   对每个不同的锚点值，剩余区间通过 LC-167 的排除规则完整扫描所有可能的两数组合，因此不会漏；
//   锚点去重避免相同第一个值重复开局，命中后左右去重避免同一锚点下重复产生相同三元组。
//
// 6. 边界与易错点
//   去重比较的是值，不是下标；锚点去重要在进入双指针前做，命中后的 left/right 去重也不能省；排序会修改输入；求和使用 long long 更稳妥。
//
// 7. 举一反三
//   这是 k-Sum 的基本降维方法：固定一个数，把 k-Sum 变成 (k-1)-Sum，最终落到有序 Two Sum。学习顺序应是 LC-167 -> LC-15，而不是分别背两套模板。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   每行输出一个三元组 "a b c"；三元组按字典序排列且不重复。
// test.in 的预期输出：-1 -1 2 | -1 0 1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<vector<int>> threeSum(vector<int>& nums) {
        sort(nums.begin(), nums.end());
        vector<vector<int>> answer;
        const int n = static_cast<int>(nums.size());

        for (int i = 0; i < n - 2; ++i) {
            if (i > 0 && nums[i] == nums[i - 1]) continue;  // 锚点去重
            if (nums[i] > 0) break;

            int left = i + 1, right = n - 1;
            while (left < right) {
                const long long sum = static_cast<long long>(nums[i]) + nums[left] + nums[right];
                if (sum < 0) {
                    ++left;
                } else if (sum > 0) {
                    --right;
                } else {
                    answer.push_back({nums[i], nums[left], nums[right]});
                    const int lv = nums[left], rv = nums[right];
                    while (left < right && nums[left] == lv) ++left;
                    while (left < right && nums[right] == rv) --right;
                }
            }
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto res = sol.threeSum(a);
    for (auto& t : res) cout << t[0] << " " << t[1] << " " << t[2] << "\n";
    return 0;
}
