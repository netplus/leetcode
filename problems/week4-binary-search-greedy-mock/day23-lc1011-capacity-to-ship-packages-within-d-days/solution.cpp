// ============================================================================
// LC-1011：在 D 天内送达包裹的能力
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 23 天
// ----------------------------------------------------------------------------
// 题目描述：
// 传送带上的包裹必须在 days 天内从一个港口运送到另一个港口。
// 传送带上的第 i 个包裹的重量为 weights[i]。
// 每一天，我们都会按给出重量（weights）的顺序往传送带上装载包裹。
// 我们装载的重量不会超过船的最大运载重量。
// 返回能在 days 天内将传送带上的所有包裹送达的船的最低运载能力。
//
// 题目示例：
// 示例 1：
//   Input: weights = [1,2,3,4,5,6,7,8,9,10], days = 5
//   Output: 15
//   Explanation: A ship capacity of 15 is the minimum to ship all the packages in 5 days like this:
//   1st day: 1, 2, 3, 4, 5
//   2nd day: 6, 7
//   3rd day: 8
//   4th day: 9
//   5th day: 10
//
//   Note that the cargo must be shipped in the order given, so using a ship of capacity 14 and splitting the packages into parts like (2, 3, 4, 5), (1, 6, 7), (8), (9), (10) is not allowed.
//
// 示例 2：
//   Input: weights = [3,2,2,4,1,4], days = 3
//   Output: 6
//   Explanation: A ship capacity of 6 is the minimum to ship all the packages in 3 days like this:
//   1st day: 3, 2
//   2nd day: 2, 4
//   3rd day: 1, 4
//
// 示例 3：
//   Input: weights = [1,2,3,1,1], days = 4
//   Output: 3
//   Explanation:
//   1st day: 1
//   2nd day: 2
//   3rd day: 3
//   4th day: 1, 1
//
// 约束与要求：
//   - 1 <= days <= weights.length <= 5 * 10^4
//   - 1 <= weights[i] <= 500
//
// 复杂度目标：O(n log S)。
//
// ----------------------------------------------------------------------------
// 解法精讲｜答案二分：把最小运载能力改写成第一个可行容量
//
// 0. 优化是怎么来的
//   一个真实且正确的直接做法是从最小可能容量 max(weights) 开始逐个尝试 capacity=maxWeight,maxWeight+1,...，对每个容量都顺序模拟装船；第一次能在 days 天内完成的容量就是答案。因为容量按从小到大枚举，所以它一定不会漏掉更小答案。问题是容量范围最坏可以延伸到 sum(weights)，每个候选又要 O(n) 模拟，总工作可能接近 O(n*S)。
//
//   真正可以省掉的是对大量连续容量的逐个尝试。固定一个 capacity 后，若它已经能在 days 天内完成，那么任何更大的容量也一定能完成；若它不能完成，那么任何更小的容量也不可能突然变得可行。于是所有候选容量天然分成：
//   false false ... false | true true ... true。
//
//   这正是 Day 22 / LC-34 的边界二分，只不过“下标 i 是否满足 nums[i]>=target”换成了“容量 C 是否能按时运完”。因此不再线性枚举容量，而是二分寻找第一个 true。优化类型：利用可行性的单调性，一次判定淘汰整段答案候选。
//
// 1. 图像直觉
//   weights = [3,2,2,4,1,4], days = 3
//
//   容量下界 = 4（至少装得下最重包裹）
//   容量上界 = 16（一天全部运完）
//
//   看两个候选：
//   capacity = 5
//   Day1: 3,2
//   Day2: 2
//   Day3: 4,1
//   Day4: 4        -> 4 天，不可行
//
//   capacity = 6
//   Day1: 3,2
//   Day2: 2,4
//   Day3: 1,4      -> 3 天，可行
//
//   容量轴可以画成：
//   4   5 | 6   7   8 ... 16
//   F   F | T   T   T ... T
//           ^
//           要找第一个 true
//
//   二分的对象不是 weights 的位置，而是“答案可能是多少”。
//
// 2. 一句话核心
//   先写出 capacity -> 是否能在 days 天内完成的单调判定，再在 [max(weights),sum(weights)] 中二分第一个可行容量。
//
// 3. 公式 / 不变量
//   搜索区间：
//   left  = max(weights)
//   right = sum(weights)
//
//   给定 capacity，顺序扫描：
//   used = 1
//   load = 0
//   若 load + weight > capacity：
//       used += 1
//       load = 0
//   随后 load += weight
//
//   feasible(capacity) := daysNeeded(capacity) <= days
//
//   单调性：
//   C1 <= C2 => daysNeeded(C1) >= daysNeeded(C2)
//   所以 feasible(C) 为 F...F | T...T。
//
//   边界二分：
//   if feasible(mid): right = mid
//   else:             left = mid + 1
//   结束 left==right，即最小可行容量。
//
// 4. 执行步骤
//   1. 先用最重单件确定 left=max(weights)，用总重量确定 right=sum(weights)，保证答案一定在闭区间内
//   2. 实现 daysNeeded(capacity)：严格按原顺序累加，当下一件放不下时才开启新一天
//   3. 每轮取 middle；若 daysNeeded(middle)<=days，middle 已可行但可能不是最小值，所以保留 middle 并令 right=middle
//   4. 若 middle 不可行，则所有更小容量也不可行，令 left=middle+1 一次丢掉整段候选
//   5. left==right 时返回该容量；它既可行，又因为左侧全被证明不可行而是最小值
//   记忆：先写 check(C)，再看是否 F...F|T...T；答案二分找第一个 true。
//
// 5. 为什么不会漏 / 不会重
//   对固定 capacity，每天尽可能晚地换天会得到最少天数：如果当前包裹还能放进今天却提前换天，只会减少今天已装重量，并不会给未来创造比‘继续装’更多容量，所以不可能减少总天数。于是 daysNeeded 是该容量下真实的最少天数。容量增大只会让原先能同一天装下的组合继续装得下，因此所需天数单调不增，可行集合必是连续后缀。边界二分始终保留第一个可行容量，最终不会漏掉更小答案。
//
// 6. 边界与易错点
//   包裹顺序不能重排，这也是 check() 必须顺序扫描的原因。left 不能从 1 开始，因为任何小于最重包裹的容量都必然无效；right=sum(weights) 一定可行。总重量和中点用 long long，避免累加溢出。可行时必须 right=middle 而不是 middle-1，因为 middle 本身可能就是第一个 true。
//
// 7. 举一反三
//   LC-410 与本题几乎同构：‘一天’换成‘一个连续子数组’，capacity 换成‘允许的最大段和’，check 仍然是按顺序尽量装满。LC-875 同样寻找答案轴上的第一个 true，但 check 改为按速度计算总耗时。以后看到‘最小化最大容量/速度/阈值’，先尝试把优化问题改成单调的可行性判定。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n D。
//   第 2 行：n 个以空格分隔的整数。
//   输出：运载能力的最小值。
// test.in 的预期输出：15
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int shipWithinDays(vector<int>& weights, int days) {
        // 容量不能小于最重单件；容量等于总重量时一天即可运完，一定可行。
        long long left = *max_element(weights.begin(), weights.end());
        long long right = accumulate(weights.begin(), weights.end(), 0LL);

        auto daysNeeded = [&](long long capacity) {
            int used = 1;
            long long load = 0;
            for (int weight : weights) {
                // 包裹顺序不能改变。固定 capacity 后，当前天能继续装就继续装；
                // 若提前换天，只会浪费当前天剩余容量，不可能减少总天数。
                if (load + weight > capacity) {
                    ++used;
                    load = 0;
                }
                // 若刚开启新的一天，触发换天的这个包裹就是新一天的第一件，不能漏掉。
                load += weight;
            }
            // 这是固定 capacity 下按原顺序运输所需的最少天数。
            return used;
        };

        // capacity 越大，daysNeeded 只会不增，所以 feasible(capacity)=[daysNeeded<=days]
        // 呈 F...F | T...T；二分寻找第一个可行容量。
        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (daysNeeded(middle) <= days) right = middle;
            else left = middle + 1;
        }
        return static_cast<int>(left);
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
    cout << sol.shipWithinDays(a, k) << "\n";
    return 0;
}
