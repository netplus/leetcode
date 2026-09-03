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
// - 核心要点：
//   1. 思路起点：排序后固定最左元素 nums[i]，剩余两数之和具有单调性，可在线性时间内找齐；三层枚举由此降为 O(n^2)。
//   2. 执行逻辑：1. 排序并枚举 i，跳过重复锚点；2. 在 i 右侧用 left/right 搜索 -nums[i]；3. 命中后同时越过两侧重复值。
//   3. 为什么这样做：对每个 i，双指针依据和的大小排除不可能端点，遍历了全部可行二元组；三处去重保证每个值三元组只输出一次。
// - 边界与易错点：条件应是三个下标两两不同；排序会修改输入；求和用 long long 防止扩展约束下溢出；nums[i]>0 时可提前结束。
// - 举一反三：k-Sum 通常递归固定一个数，最终降到有序 Two Sum；去重位置与本题完全同源。
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

