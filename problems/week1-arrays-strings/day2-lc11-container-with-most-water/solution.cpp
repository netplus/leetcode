// ============================================================================
// LC-11：盛最多水的容器
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 1 周 / 第 2 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个长度为 n 的整数数组 height。
// 有 n 条垂线，第 i 条线的两个端点是 (i, 0) 和 (i, height[i])。
// 找出其中的两条线，使得它们与 x 轴共同构成的容器可以容纳最多的水。
// 返回容器可以储存的最大水量。
// 说明：你不能倾斜容器。
//
// 题目示例：
// 示例 1：
//   Input: height = [1,8,6,2,5,4,8,3,7]
//   Output: 49
//   Explanation: The above vertical lines are represented by array [1,8,6,2,5,4,8,3,7]. In this case, the max area of water (blue section) the container can contain is 49.
//
// 示例 2：
//   Input: height = [1,1]
//   Output: 1
//
// 约束与要求：
//   - n == height.length
//   - 2 <= n <= 10^5
//   - 0 <= height[i] <= 10^4
//
// 复杂度目标：O(n) 时间，O(1) 空间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜相向双指针：移动短板
//
// 0. 优化是怎么来的
//   最直接的做法是枚举所有左右边界 (left,right)，计算每个容器面积，时间 O(n^2)。
//
//   优化的关键是看清一次比较能永久淘汰谁。若左板更短，那么当前面积已经使用了“保留这块左板时能够得到的最大宽度”；以后只移动右边界，宽度一定更小，而有效高度仍不可能超过这块左板，所以这些候选全部被当前状态支配。
//
//   因此真正可尝试的方向只有“换掉短板”。每轮都能永久删除一个端点，O(n^2) 的端点组合枚举被压缩成 O(n) 次安全淘汰。
//
//   优化类型：候选被支配 -> 双指针贪心淘汰。
//
// 1. 图像直觉
//   把左右两条线当成水桶的两块板：
//
//   height[left] = 3                 height[right] = 8
//         |                               |
//         |~~~~~~~~ 水面只能到 3 ~~~~~~~~|
//         |                               |
//         L-------------------------------R
//
//   面积的高度被短板 3 卡死。
//   如果保留左边这块短板，只把 right 往左移动：宽度一定变小，而水面上限仍然不可能超过 3。
//   所以所有“继续使用这块左短板”的更窄容器都不可能比当前更大，可以一次性丢掉 left。
//
//   想让面积还有变大的可能，只能尝试换掉短板，看能不能找到更高的板。
//
// 2. 一句话核心
//   宽度每次都会缩小，因此只有换掉当前短板，才有机会提高决定面积的有效高度。
//
// 3. 公式 / 不变量
//   area = (right-left) * min(height[left], height[right])
//
//   若 height[left] <= height[right]：
//   对任意 right' < right，
//   min(height[left], height[right']) <= height[left]
//   且 right'-left < right-left
//   => area(left,right') <= 当前 area
//   => left 可以安全排除
//
// 4. 执行步骤
//   1. left/right 从最宽的两端开始
//   2. 计算当前面积并更新 best
//   3. 比较两端高度，移动较短的一端；相等时移动任意一端
//   4. 重复直到 left==right
//   记忆：先算面积；谁是短板，就丢谁。
//
// 5. 为什么不会漏 / 不会重
//   当左边是短板时，所有保留 left、只缩短右边界的候选都被当前组合支配：宽度更小，且高度上限不会超过 height[left]。
//   因此丢弃 left 不会错过更优解；右边较短时完全对称。
//
// 6. 边界与易错点
//   不要移动较高的一端，那无法突破当前短板上限；宽度是 right-left；面积乘法最好先提升到 long long；
//   这题虽然也是相向双指针，但依据是“瓶颈支配关系”，不同于 LC-167 的有序和单调性。
//
// 7. 举一反三
//   看到“两个端点决定答案，且其中较弱的一端形成上限”时，可以尝试寻找类似的支配关系。关键是先证明丢掉哪一端安全，再写双指针。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最大面积。
// test.in 的预期输出：49
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int maxArea(vector<int>& height) {
        int left = 0, right = static_cast<int>(height.size()) - 1;
        long long best = 0;
        while (left < right) {
            const int limitingHeight = min(height[left], height[right]);
            best = max(best, 1LL * limitingHeight * (right - left));
            if (height[left] <= height[right]) ++left;  // 只有短板变化才可能改善
            else --right;
        }
        return static_cast<int>(best);
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
    cout << sol.maxArea(a) << "\n";
    return 0;
}
