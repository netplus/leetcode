// ============================================================================
// LC-239：滑动窗口最大值
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 7 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums，有一个大小为 k 的滑动窗口从数组的最左侧移动到数组的最右侧。
// 你只可以看到在滑动窗口内的 k 个数字。
// 滑动窗口每次只向右移动一位。
// 返回滑动窗口中的最大值。
//
// 约束与要求：
//   - 1 <= nums.length <= 10^5
//   - -10^4 <= nums[i] <= 10^4
//   - 1 <= k <= nums.length
//
// 复杂度目标：O(n) 时间（单调队列）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜单调队列：保存仍可能成为最大值的下标
// - 核心要点：
//   1. 思路起点：双端队列中的下标对应值严格递减；新值进入时，队尾所有不大于它的值永远不可能再当最大值，可删除。
//   2. 执行逻辑：1. 移除已滑出窗口的队首；2. 从队尾删除不大于当前值的下标，再压入当前下标；3. 窗口形成后输出队首值。
//   3. 为什么这样做：队首始终是窗口内最大候选；被队尾淘汰的元素更小且更早过期，不可能优于新元素。每个下标最多入队出队各一次。
// - 边界与易错点：队列必须存下标才能判断过期；重复值用 <= 淘汰旧值可让新值存活更久；输出从 i>=k-1 开始。
// - 举一反三：单调队列适合滑窗最值、带宽限制的 DP 优化，以及前缀和上寻找最优候选。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n k。
//   第 2 行：n 个以空格分隔的整数。
//   输出：n-k+1 个以空格分隔的窗口最大值。
// test.in 的预期输出：3 3 5 5 6 7
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    vector<int> maxSlidingWindow(vector<int>& nums, int k) {
        deque<int> candidates;
        vector<int> answer;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            if (!candidates.empty() && candidates.front() <= i - k) {
                candidates.pop_front();
            }
            while (!candidates.empty() && nums[candidates.back()] <= nums[i]) {
                candidates.pop_back();
            }
            candidates.push_back(i);
            if (i >= k - 1) answer.push_back(nums[candidates.front()]);
        }
        return answer;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, k; if (!(cin >> n >> k)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    auto ans = sol.maxSlidingWindow(a, k);
    for (size_t i = 0; i < ans.size(); ++i) cout << ans[i] << " \n"[i + 1 == ans.size()];
    return 0;
}

