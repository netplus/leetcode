// ============================================================================
// LC-4：寻找两个正序数组的中位数
// 难度：困难
// 优先级：P1（进阶）
// 学习进度：第 4 周 / 第 23 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定两个大小分别为 m 和 n 的正序（从小到大）数组 nums1 和 nums2。
// 请你找出并返回这两个正序数组的中位数。
// 算法的时间复杂度应该为 O(log (m+n))。
//
// 约束与要求：
//   - nums1.length == m
//   - nums2.length == n
//   - 0 <= m <= 1000
//   - 0 <= n <= 1000
//   - 1 <= m + n <= 2000
//   - -10^6 <= nums1[i], nums2[i] <= 10^6
//
// 复杂度目标：O(log(min(m,n))) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜较短数组上二分分割线
// - 核心要点：
//   1. 思路起点：把两个有序数组分别切成左、右两部分，使左边总元素数固定且左侧最大值<=右侧最小值；中位数只由四个边界值决定。
//   2. 执行逻辑：1. 确保 nums1 是较短数组；2. 二分 cut1，并由总左长度推出 cut2；3. 若分割合法计算中位数，否则按边界大小移动 cut1。
//   3. 为什么这样做：合法分割保证合并序列所有左元素不大于所有右元素，且数量平衡，所以奇数时左侧最大是中位数，偶数时两侧边界均值是中位数。
// - 边界与易错点：空侧用 INT_MIN/INT_MAX 哨兵；奇偶统一用 (m+n+1)/2；平均前转 long long 避免整数加法溢出。
// - 举一反三：两个有序集合的第 k 小也可通过分割数量和交叉边界条件求解，是本题的更一般形式。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：m n。
//   第 2 行：m 个以空格分隔的整数 (nums1)。
//   第 3 行：n 个以空格分隔的整数 (nums2)。
//   输出：中位数（以浮点数表示，例如 2.0 或 2.5）。
// test.in 的预期输出：2.5
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
        const int m = static_cast<int>(nums1.size());
        const int n = static_cast<int>(nums2.size());
        const int leftSize = (m + n + 1) / 2;
        int left = 0, right = m;

        while (left <= right) {
            const int cut1 = left + (right - left) / 2;
            const int cut2 = leftSize - cut1;
            const int left1 = cut1 == 0 ? INT_MIN : nums1[cut1 - 1];
            const int right1 = cut1 == m ? INT_MAX : nums1[cut1];
            const int left2 = cut2 == 0 ? INT_MIN : nums2[cut2 - 1];
            const int right2 = cut2 == n ? INT_MAX : nums2[cut2];

            if (left1 <= right2 && left2 <= right1) {
                const int leftMax = max(left1, left2);
                if ((m + n) % 2 == 1) return leftMax;
                const int rightMin = min(right1, right2);
                return (static_cast<long long>(leftMax) + rightMin) / 2.0;
            }
            if (left1 > right2) right = cut1 - 1;
            else left = cut1 + 1;
        }
        return 0.0;  // 两个输入有序且总长度非零时不会到达
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int m, n; if (!(cin >> m >> n)) return 0;
    vector<int> a(m), b(n);
    for (int i = 0; i < m; ++i) cin >> a[i];
    for (int i = 0; i < n; ++i) cin >> b[i];
    cout << sol.findMedianSortedArrays(a, b) << "\n";
    return 0;
}

