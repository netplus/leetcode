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
// 解法精讲｜双指针接雨水：由较低侧上界确定
// - 核心要点：
//   1. 思路起点：leftMax/rightMax 分别是两侧已见最高柱；哪侧上界较小，该侧可接水量已经由它确定，不受另一侧更远柱影响。
//   2. 执行逻辑：1. 左右指针从两端向内；2. 比较 leftMax 与 rightMax；3. 处理较小上界侧：更新最大值或累加 max-height，然后移动。
//   3. 为什么这样做：当 leftMax<=rightMax，左位置右侧至少存在高度 rightMax，故水位上限由 leftMax 唯一决定；处理后排除该位置安全，另一侧对称。
// - 边界与易错点：先更新边界再计算差可避免负数；宽度单位为 1；空/少于三柱自然接不到水。
// - 举一反三：双侧边界最值常可把前后缀数组压成双指针，例如接水、容器和边界约束扫描。
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

