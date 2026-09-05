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
// 题目示例：
// 示例 1：
//   Input: nums1 = [1,3], nums2 = [2]
//   Output: 2.00000
//   Explanation: merged array = [1,2,3] and median is 2.
//
// 示例 2：
//   Input: nums1 = [1,2], nums2 = [3,4]
//   Output: 2.50000
//   Explanation: merged array = [1,2,3,4] and median is (2 + 3) / 2 = 2.5.
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
// 解法精讲｜有序数组分割二分：搜索一条让左右数量平衡且值域合法的切线
//
// 0. 优化是怎么来的
//   最自然且正确的算法是像归并排序那样合并两个有序数组，再直接读取中间一个或两个元素，时间 O(m+n)、额外空间 O(m+n)；即使不真正保存结果，只用双指针走到中间，也仍需要 O(m+n) 的最坏时间。
//
//   这里浪费的是：为了得到中位数，我们处理了大量远离中间边界的元素，而真正决定答案的只有“整体左半最大的数”和“整体右半最小的数”。由于两个输入内部已经有序，可以不构造完整归并序列，而是直接寻找一条分割线：左侧元素数量固定，且左边所有值都不大于右边所有值。
//
//   若 nums1 切出 cut1 个元素，那么 nums2 必须切出 leftSize-cut1 个，第二个切点无需搜索。于是只剩一个变量 cut1。若 left1>right2，说明 nums1 左边拿多了，cut1 必须左移；若 left2>right1，说明 nums1 左边拿少了，cut1 必须右移。这种方向性让 cut1 可以二分。优化类型：利用两个数组内部有序性，把完整归并压缩成对一个分割边界的搜索。
//
// 1. 图像直觉
//   先保证 nums1 是较短数组。
//   例如 nums1=[2]，nums2=[1,3]，总长度 3：
//   leftSize = (1+2+1)/2 = 2
//
//   尝试：
//   nums1: [2] | +∞
//   nums2: [1] | [3]
//           ----+----
//   左侧共有 2 个元素。
//
//   四个边界：
//   left1=2   right1=+∞
//   left2=1   right2=3
//
//   交叉检查：
//   left1 <= right2   => 2 <= 3
//   left2 <= right1   => 1 <= +∞
//   都成立。
//
//   因此整体排序后的左半最大值就是 max(2,1)=2。
//   总长度为奇数，中位数直接是 2。
//
//   若出现 left1 > right2：
//   nums1 左边拿得太多 -> cut1 左移。
//   若出现 left2 > right1：
//   nums1 左边拿得太少 -> cut1 右移。
//
// 2. 一句话核心
//   固定整体左半应有多少元素，只二分较短数组的切点；另一个切点随之确定，直到两侧数量平衡且两个交叉边界都满足左<=右。
//
// 3. 公式 / 不变量
//   令 m<=n：
//   leftSize = (m+n+1)/2
//   cut1 ∈ [0,m]
//   cut2 = leftSize - cut1
//
//   边界值：
//   left1  = cut1==0 ? -∞ : nums1[cut1-1]
//   right1 = cut1==m ? +∞ : nums1[cut1]
//   left2  = cut2==0 ? -∞ : nums2[cut2-1]
//   right2 = cut2==n ? +∞ : nums2[cut2]
//
//   合法分割：
//   left1 <= right2 && left2 <= right1
//
//   若 left1 > right2：cut1 太大，right=cut1-1
//   否则若 left2 > right1：cut1 太小，left=cut1+1
//
//   合法时：
//   奇数总长：median = max(left1,left2)
//   偶数总长：median = (max(left1,left2)+min(right1,right2))/2。
//
// 4. 执行步骤
//   1. 若 nums1 比 nums2 长，交换参数角色，始终在较短数组上二分，使复杂度为 O(log min(m,n))，并保证由 cut1 推出的 cut2 落在合法范围
//   2. 计算固定的 leftSize=(m+n+1)/2，在 cut1 的闭区间 [0,m] 上二分；每个 cut1 唯一确定 cut2
//   3. 用 INT_MIN/INT_MAX 表示切口落在数组端点时不存在的左/右边界，这样无需为四种空侧分别写分支
//   4. 若两个交叉条件都成立，分割线已经对应整体有序序列的中间边界，按总长度奇偶直接返回中位数
//   5. 若 left1>right2，nums1 左侧取多了就左移 cut1；否则必是 left2>right1，nums1 左侧取少了就右移 cut1
//   记忆：不合并整表，只找中间切线：左边数量固定，交叉边界都满足左<=右。
//
// 5. 为什么不会漏 / 不会重
//   每个数组内部有序，因此若 left1<=right2 且 left2<=right1，则 nums1 左半的所有元素都<=nums2右半，nums2左半的所有元素也都<=nums1右半；再加上各自内部顺序，可知合并后的所有左侧元素都不大于所有右侧元素。leftSize 又保证左侧数量恰好是整体的一半（奇数时多一个），所以中位数只可能是左侧最大值或左右边界均值。若 left1>right2，增加 cut1 只会让 nums1 左边界不减、nums2 的 cut2 更小，因此方向只会更错，必须左移；另一种冲突对称地必须右移，所以二分不会丢失合法切点。
//
// 6. 边界与易错点
//   这题不是“答案二分”，不要看到 Day 23 就强行套 feasible(value)。必须先让 nums1 成为较短数组；否则 cut2 可能越界。leftSize 用 (m+n+1)/2 可以统一奇偶，并让奇数时多出的元素放在左侧。端点哨兵只是比较用的虚拟值，不是真实数据。偶数中位数求和前转 long long，避免两个 int 边界相加溢出。
//
// 7. 举一反三
//   该分割模型可以推广到两个有序数组的第 k 小：把 leftSize 从中位数所需数量改成 k，再寻找满足交叉边界的切点。它与 Day 22/23 其它二分题共享的不是业务模板，而是更底层原则——找到一个可单调纠正的边界变量，每次比较证明搜索方向。
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
        // 始终在较短数组上二分 cut1。这样 cut1∈[0,m] 时，
        // 由固定左侧总元素数推导出的 cut2 也会落在 [0,n]，避免额外越界分支。
        if (nums1.size() > nums2.size()) return findMedianSortedArrays(nums2, nums1);
        const int m = static_cast<int>(nums1.size());
        const int n = static_cast<int>(nums2.size());

        // cut1/cut2 表示“左半部分各取多少个元素”，不是数组下标。
        // +1 让总长度为奇数时，多出来的那个中位元素统一放在左半部分。
        const int leftSize = (m + n + 1) / 2;
        int left = 0, right = m;

        while (left <= right) {
            const int cut1 = left + (right - left) / 2;
            const int cut2 = leftSize - cut1;

            // 切口落在数组端点时，用 ±∞ 作为“空侧”的虚拟边界，
            // 这样所有切法都能使用同一组交叉比较，不必为首尾单独分支。
            const int left1 = cut1 == 0 ? INT_MIN : nums1[cut1 - 1];
            const int right1 = cut1 == m ? INT_MAX : nums1[cut1];
            const int left2 = cut2 == 0 ? INT_MIN : nums2[cut2 - 1];
            const int right2 = cut2 == n ? INT_MAX : nums2[cut2];

            // 两个交叉不等式同时成立，才说明“合并后的所有左侧元素 <= 所有右侧元素”。
            if (left1 <= right2 && left2 <= right1) {
                const int leftMax = max(left1, left2);
                // 奇数时左半多一个元素，中位数就是左侧最大值。
                if ((m + n) % 2 == 1) return leftMax;
                const int rightMin = min(right1, right2);
                // 偶数时取两个中间边界的平均；先提升为 long long，避免两个 int 相加溢出。
                return (static_cast<long long>(leftMax) + rightMin) / 2.0;
            }

            // left1>right2 表示 nums1 左侧拿得太多，cut1 必须左移；
            // 否则必是 left2>right1，说明 nums1 左侧拿得太少，cut1 必须右移。
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

