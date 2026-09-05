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
// 题目示例：
// 示例 1：
//   Input: piles = [3,6,7,11], h = 8
//   Output: 4
//
// 示例 2：
//   Input: piles = [30,11,23,4,20], h = 5
//   Output: 30
//
// 示例 3：
//   Input: piles = [30,11,23,4,20], h = 6
//   Output: 23
//
// 约束与要求：
//   - 1 <= piles.length <= 10^4
//   - piles.length <= h <= 10^9
//   - 1 <= piles[i] <= 10^9
//
// 复杂度目标：O(n log max(piles))。
//
// ----------------------------------------------------------------------------
// 解法精讲｜答案二分：把最小吃速转成速度轴上的第一个可行点
//
// 0. 优化是怎么来的
//   一个自然且正确的做法是按 speed=1,2,3,... 逐个尝试，计算每个速度需要的总小时数，第一次满足 hours<=h 就返回。因为速度按升序枚举，所以得到的一定是最小可行速度；但 piles[i] 可以到 10^9，逐个试速度会浪费巨大范围。
//
//   关键观察是：每堆所需小时 ceil(pile/speed) 随 speed 增大单调不增，因此总小时数也单调不增。如果某个 speed 已能在 h 小时内完成，那么任何更快速度都可行；如果它超时，那么更慢速度一定也超时。
//
//   所以速度候选再次形成 false...false | true...true。和 LC-1011 相比，二分边界模板完全不用改，只需替换 check(speed) 的业务含义。优化类型：利用答案可行性的单调性，跳过大量无需逐个测试的速度。
//
// 1. 图像直觉
//   piles = [3,6,7,11], h = 8
//
//   speed = 3：
//   ceil(3/3)+ceil(6/3)+ceil(7/3)+ceil(11/3)
//   = 1+2+3+4 = 10 小时 -> 不可行
//
//   speed = 4：
//   1+2+2+3 = 8 小时 -> 可行
//
//   速度轴：
//   1  2  3 | 4  5 ... 11
//   F  F  F | T  T ... T
//             ^
//             最小可行速度
//
//   这里不能把所有香蕉总数直接除以 speed：
//   每小时只能选择一堆，剩余不足 speed 的那一小时也不能转去吃另一堆。
//   所以每一堆必须分别向上取整。
//
// 2. 一句话核心
//   把 speed 当作答案候选，使用 sum(ceil(pile/speed))<=h 做单调 check，然后二分速度轴上的第一个 true。
//
// 3. 公式 / 不变量
//   搜索区间：
//   left = 1
//   right = max(piles)
//
//   hours(speed) = Σ ceil(pile / speed)
//                = Σ (pile + speed - 1) / speed
//
//   feasible(speed) := hours(speed) <= h
//
//   若 s1 <= s2：
//   ceil(pile/s1) >= ceil(pile/s2)
//   => hours(s1) >= hours(s2)
//   => feasible 为 F...F | T...T。
//
//   if feasible(mid): right = mid
//   else:             left = mid + 1。
//
// 4. 执行步骤
//   1. 速度至少为 1，最大只需 max(piles)：达到最大堆大小后，每堆最多一小时，而题目保证 h>=piles.length，因此上界一定可行
//   2. 实现 canFinish(speed)，对每一堆分别计算向上取整小时数并累加到 long long hours
//   3. 若累计 hours 已经超过 h，可以立即返回 false；后续只会继续增加，没有必要再扫描
//   4. middle 可行时保留它并令 right=middle；不可行时把 middle 及其左侧全部淘汰，令 left=middle+1
//   5. left==right 时返回最小可行整数速度
//   记忆：答案不是数组下标也能二分：只要 speed 越大越容易，就找速度轴第一个 true。
//
// 5. 为什么不会漏 / 不会重
//   对每一堆，速度越大，完成该堆需要的整小时数不会增加；逐堆相加后总耗时同样单调不增，因此可行速度一定构成连续后缀。二分只在已证明不可行时删除左段，在已知可行时保留 middle 继续寻找更小值，所以收敛点正是第一个可行速度。每堆单独向上取整准确反映“一小时只处理一堆且剩余时间不能转移”的题意。
//
// 6. 边界与易错点
//   最常见错误是用 ceil(sum(piles)/speed)，这错误地允许一小时在多堆之间转移剩余吞吐量；必须逐堆取整后相加。表达式先把 pile 转成 long long，避免 pile+speed-1 的 int 溢出风险。可行时 right=middle，不能跳过 middle。
//
// 7. 举一反三
//   和 LC-1011/410 一样，真正可复用的是“候选答案 -> O(n) check -> 单调真假边界”三件套，而不是某个业务公式。运输容量、加工速度、最小阈值、最大允许负载都可以先尝试在答案轴上建立单调谓词。
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
        // 速度至少为 1；取最大堆大小时，每堆最多一小时，因此一定可行。
        // 所以最小可行速度必位于闭区间 [1,maxPile]。
        int left = 1, right = *max_element(piles.begin(), piles.end());

        // canFinish(speed) 是答案轴上的单调谓词：速度越大，总耗时只会不增。
        auto canFinish = [&](int speed) {
            long long hours = 0;
            for (int pile : piles) {
                // 每一堆必须单独向上取整：一个小时只能处理当前这一堆，
                // 吃完后剩余的“小时容量”不能转移到下一堆。
                // 先提升 pile 到 long long，避免 pile+speed-1 的 int 加法溢出。
                hours += (static_cast<long long>(pile) + speed - 1) / speed;

                // 一旦已经超过 h，后面的堆只会继续增加耗时，可以提前判定不可行。
                if (hours > h) return false;
            }
            return true;
        };

        // 可行速度构成连续后缀 F...F | T...T，寻找第一个 true。
        while (left < right) {
            int middle = left + (right - left) / 2;
            // middle 可行时它仍可能就是最小答案，所以保留 middle；
            // 不可行时所有更小速度同样不可行，可一次全部丢弃。
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

