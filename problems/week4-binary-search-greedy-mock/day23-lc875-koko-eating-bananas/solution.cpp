// ============================================================================
// LC-875：爱吃香蕉的珂珂
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 23 天
// ----------------------------------------------------------------------------
// 题目描述：
// 珂珂喜欢吃香蕉。
// 这里有 n 堆香蕉，第 i 堆中有 piles[i] 根香蕉。
// 警卫已经离开了，将在 h 小时后回来。
// 珂珂可以决定她吃香蕉的速度 k （单位：根/小时）。
// 每个小时，她将会选择一堆香蕉，从中吃掉 k 根。
// 如果这堆香蕉少于 k 根，她将吃掉这堆的所有香蕉，然后这一小时内不会再吃更多的香蕉。
// 珂珂喜欢慢慢吃，但仍然想在警卫回来前吃掉所有的香蕉。
// 返回她可以在 h 小时内吃掉所有香蕉的最小速度 k （k 为整数）。
//
// 约束与要求：
//   - 1 <= piles.length <= 10^4
//   - piles.length <= h <= 10^9
//   - 1 <= piles[i] <= 10^9
//
// 复杂度目标：O(n log max(piles))。
//
// ----------------------------------------------------------------------------
// 解法精讲｜答案二分：最小可行速度
// - 核心要点：
//   1. 思路起点：速度 speed 越大，总耗时 sum(ceil(pile/speed)) 越小；“能否在 h 小时内吃完”是单调谓词。
//   2. 执行逻辑：1. 速度范围设为 [1,maxPile]；2. 计算中点速度所需总小时并可提前截断；3. 可行时保留中点收缩右界，不可行时抬高左界。
//   3. 为什么这样做：二分始终保留最小可行速度；可行集合是连续后缀，left==right 时唯一边界就是答案。
// - 边界与易错点：向上取整用 (pile+speed-1)/speed；小时总数用 long long；每小时只能处理一堆，所以各堆分别取整。
// - 举一反三：凡是资源速率增大使完成时间单调下降的问题，都可对速率做最小可行值二分。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n h。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最小吃香蕉速度。
// test.in 的预期输出：4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int minEatingSpeed(vector<int>& piles, int h) {
        int left = 1, right = *max_element(piles.begin(), piles.end());
        auto canFinish = [&](int speed) {
            long long hours = 0;
            for (int pile : piles) {
                hours += (static_cast<long long>(pile) + speed - 1) / speed;
                if (hours > h) return false;
            }
            return true;
        };
        while (left < right) {
            int middle = left + (right - left) / 2;
            if (canFinish(middle)) right = middle;
            else left = middle + 1;
        }
        return left;
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
    cout << sol.minEatingSpeed(a, k) << "\n";
    return 0;
}

