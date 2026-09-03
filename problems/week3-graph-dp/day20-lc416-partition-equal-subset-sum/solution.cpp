// ============================================================================
// LC-416：分割等和子集
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 3 周 / 第 20 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个只包含正整数的非空数组 nums。
// 请你判断是否可以将这个数组分割成两个子集，使得两个子集的元素和相等。
//
// 约束与要求：
//   - 1 <= nums.length <= 200
//   - 1 <= nums[i] <= 100
//
// 复杂度目标：O(n*sum) 时间，O(sum) 空间（0/1 背包）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜0/1 背包：是否能达到半和
// - 核心要点：
//   1. 思路起点：总和为奇数不可能等分；否则问题变成从每个元素最多选一次，能否凑出 target=sum/2。
//   2. 执行逻辑：1. 计算总和并排除奇数；2. dp[0]=true；3. 对每个 value 从 target 向下更新 dp[s]|=dp[s-value]。
//   3. 为什么这样做：倒序保证转移只读取加入当前元素之前的状态，所以每项至多使用一次；dp[target] 真当且仅当存在目标子集，其补集和也相等。
// - 边界与易错点：容量必须倒序，否则会变成完全背包重复使用元素；可在 dp[target] 为真时提前返回。
// - 举一反三：子集和、等分集合和资源选择都是 0/1 背包；布尔、计数、最大价值只改变 dp 的值域和聚合操作。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：能分割为两个等和子集时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    bool canPartition(vector<int>& nums) {
        const int total = accumulate(nums.begin(), nums.end(), 0);
        if (total % 2 != 0) return false;
        const int target = total / 2;
        vector<char> reachable(target + 1, false);
        reachable[0] = true;
        for (int value : nums) {
            for (int sum = target; sum >= value; --sum) {
                reachable[sum] = reachable[sum] || reachable[sum - value];
            }
        }
        return reachable[target];
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
    cout << (sol.canPartition(a) ? 1 : 0) << "\n";
    return 0;
}

