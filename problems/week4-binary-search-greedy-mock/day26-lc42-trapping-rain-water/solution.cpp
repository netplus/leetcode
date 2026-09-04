// ============================================================================
// LC-42：接雨水
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 26 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定 n 个非负整数表示每个宽度为 1 的柱子的高度图，计算按此排列的柱子，下雨之后能接多少雨水。
//
// 约束与要求：
//   - n == height.length
//   - 1 <= n <= 2 * 10^4
//   - 0 <= height[i] <= 10^5
//
// 复杂度目标：O(n) 时间，O(1) 空间（双指针）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜双侧边界压缩：从每格左右扫描到 O(1) 双指针
//
// 0. 优化是怎么来的
//   最直接的正确算法是对每个位置 i，分别向左扫描最高柱、向右扫描最高柱，再计算 min(leftMax,rightMax)-height[i]。每个位置都重新寻找几乎相同的两侧最大值，时间 O(n^2)。
//
//   第一步可复用这些查询：预处理 prefixMax[i] 和 suffixMax[i]，每个位置 O(1) 得到两侧最高柱，总时间 O(n)，但额外空间 O(n)。继续观察这些数组真正提供的信息：处理左端时，只要 leftMax<=rightMax，就已经知道右侧至少存在一根高度为 rightMax 的柱，因此左端水位一定由较小的 leftMax 决定；右侧具体还有多高不重要。于是只需一边向内走一边维护 leftMax/rightMax，把两个 O(n) 边界数组压缩成四个标量。优化类型：重复计算消除 -> 状态可延续 -> 边界状态压缩。
//
// 1. 图像直觉
//   height = [0,1,0,2]
//
//   对中间的 0：
//   左最高 = 1
//   右最高 = 2
//   水位 = min(1,2)=1，接 1
//
//   双指针视角：
//   leftMax=1 <= rightMax=2
//             ^ 较低边界已经是瓶颈
//
//   右边哪怕不是 2 而是 20，
//   这个左位置最多仍只能蓄到高度 1。
//   所以它现在就可以结算，不必等待右侧扫描完成。
//
// 2. 一句话核心
//   始终结算两侧已知最高边界中较低的一侧，因为较低侧已经成为该位置水位的确定瓶颈。
//
// 3. 公式 / 不变量
//   单点公式：
//   water[i] = max(0, min(maxLeft[i], maxRight[i]) - height[i])
//
//   双指针维护：
//   leftMax  = 已扫描左侧最高柱
//   rightMax = 已扫描右侧最高柱
//
//   若 leftMax <= rightMax：
//       更新 leftMax
//       water += leftMax - height[left]
//       left++
//   否则对称处理 right。
//
//   不变量：被移出 [left,right] 的位置，其最终水量已经被唯一确定并累计。
//
// 4. 执行步骤
//   1. left/right 从两端开始，leftMax/rightMax 记录两侧已经见过的最高柱
//   2. 若 leftMax<=rightMax，先用 height[left] 更新 leftMax，再累计 leftMax-height[left]，然后 left++
//   3. 否则对右侧做完全对称的更新、累计和 right--
//   4. 直到区间处理完；water 即所有位置水量之和
//   记忆：水位看两墙短的那面；短墙一旦确定，这一格就可以结算。
//
// 5. 为什么不会漏 / 不会重
//   当 leftMax<=rightMax 时，右侧已经存在一根高度至少为 rightMax 的真实柱，因此对 left 位置而言，右边界绝不会低于 leftMax；它的最终水位只可能由 leftMax 限制。先把当前柱纳入 leftMax 后，leftMax-height[left] 就是该位置最终水量，可以安全永久结算。另一分支完全对称。每轮恰好移走一个尚未结算的位置，所以所有位置都会被处理一次且不会重复。
//
// 6. 边界与易错点
//   不要把比较条件机械换成 height[left] 与 height[right] 后仍沿用同一套变量证明；当前实现比较的是 leftMax/rightMax。先更新边界再做差，才能保证贡献非负。少于三根柱子自然得到 0。water 用 long long 累加更稳妥，最终按题目返回 int。
//
// 7. 举一反三
//   这条推导比直接背双指针更重要：先把 O(n^2) 重复边界查询变成前后缀缓存，再问缓存中哪些信息可以在线延续、何时一侧答案已被较小边界确定。类似的“双侧上界谁先确定谁”思路会出现在边界约束扫描中；与 LC-11 盛最多水的容器相比，两题都移动较弱边界，但淘汰证明不同。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：可接雨水的总量。
// test.in 的预期输出：6
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int trap(vector<int>& height) {
        int left = 0, right = static_cast<int>(height.size()) - 1;
        int leftMax = 0, rightMax = 0;
        long long water = 0;
        while (left <= right) {
            if (leftMax <= rightMax) {
                leftMax = max(leftMax, height[left]);
                water += leftMax - height[left];
                ++left;
            } else {
                rightMax = max(rightMax, height[right]);
                water += rightMax - height[right];
                --right;
            }
        }
        return static_cast<int>(water);
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
    cout << sol.trap(a) << "\n";
    return 0;
}
