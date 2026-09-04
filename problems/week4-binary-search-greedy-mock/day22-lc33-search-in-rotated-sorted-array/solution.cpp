// ============================================================================
// LC-33：搜索旋转排序数组
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 22 天
// ----------------------------------------------------------------------------
// 题目描述：
// 整数数组 nums 按升序排列，数组中的值互不相同。
// 在传递给函数之前，nums 在预先未知的某个下标 k （0 <= k < nums.length）上进行了向左旋转，使数组变为 [nums[k], nums[k+1], ..., nums[n-1],
// nums[0], nums[1], ..., nums[k-1]] （下标从 0 开始计数）。
// 例如，[0,1,2,4,5,6,7] 下标 3 上向左旋转后可能变为 [4,5,6,7,0,1,2]。
// 给你旋转后的数组 nums 和一个整数 target，如果 nums 中存在这个目标值 target，则返回它的下标，否则返回 -1。
// 你必须设计一个时间复杂度为 O(log n) 的算法解决此问题。
//
// 约束与要求：
//   - 1 <= nums.length <= 5000
//   - -10^4 <= nums[i] <= 10^4
//   - nums 中的每个值都独一无二
//   - 题目数据保证 nums 在预先未知的某个下标上进行了旋转
//   - -10^4 <= target <= 10^4
//
// 复杂度目标：O(log n) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜旋转数组二分：整体失序时，每轮先找到仍然有序的那一半
//
// 0. 优化是怎么来的
//   最自然正确的方法是线性扫描 target，O(n)。普通 LC-704 二分不能直接套，因为 nums[mid]<target 并不再意味着 target 一定在右边：例如 [4,5,6,7,0,1,2] 中 middle=7，而小值都在旋转断点之后。
//
//   关键不是强求整个候选区间单调，而是问：一次比较还能否找到一块‘确定有序、因此可以整段判断’的区域？旋转后的两个升序片段只在一个断点处失序；在元素互异的前提下，中点不可能让左右两侧都跨过那个唯一断点，所以至少一半保持升序。
//
//   如果左半有序，就能用 nums[left] <= target < nums[middle] 精确判断 target 是否属于左半值域；属于则保留左半，否则整段左半可淘汰。若左半无序，则右半必有序，做对称判断。这样每轮仍能删除约一半候选，恢复 O(log n)。
//
// 1. 图像直觉
//   nums = [4,5,6,7,0,1,2], target=0
//   left=0        mid=3        right=6
//   [4,5,6,7 | 0,1,2]
//    ^       ^   ^     ^
//
//   [left..mid] = [4,5,6,7] 明确有序。
//   它的值域是 [4,7)，target=0 不在里面，
//   所以即使我们还不知道旋转点的精确位置，也能一次删掉整个左半：
//
//   [4,5,6,7] | [0,1,2]
//    x x x x     ^ 继续搜索
//
//   核心不是先找 pivot，而是每轮抓住一半可证明的有序结构。
//
// 2. 一句话核心
//   每轮先判断哪一半仍有序，再判断 target 是否落在这半的值域；若不在，就整半淘汰。
//
// 3. 公式 / 不变量
//   闭区间 [left,right] 内，先检查 nums[middle]==target。
//
//   若 nums[left] <= nums[middle]：左半有序
//       nums[left] <= target < nums[middle]
//           -> right = middle-1
//       否则
//           -> left = middle+1
//
//   否则：右半有序
//       nums[middle] < target <= nums[right]
//           -> left = middle+1
//       否则
//           -> right = middle-1
//
//   不变量：target 若存在，始终保留在 [left,right]；每轮至少删除 middle 和一侧候选。
//
// 4. 执行步骤
//   1. 用 LC-704 相同的闭区间 left/right 和 middle；若中点直接命中，立即返回
//   2. 比较 nums[left] 与 nums[middle]：若前者 <= 后者，则左半确定有序，否则右半确定有序
//   3. 在确定有序的一半上，用端点值判断 target 是否位于其值域；位于则只保留该半，否则删除该半
//   4. 更新边界时始终越过 middle；候选区间为空后返回 -1
//   记忆：整体不有序没关系：先找本轮确定有序的一半，再用值域决定保留哪边。
//
// 5. 为什么不会漏 / 不会重
//   元素互异且候选区间最多跨一个旋转断点，因此 middle 两侧至少一侧不跨断点、保持升序。对于这段有序区间，端点值可以准确判定 target 是否属于其值域：若属于，答案只能留在这一半；若不属于，即使 target 存在也只能在另一半。每轮只删除已被值域证明不可能的一半，所以不会漏；middle 每轮被命中或排除，区间严格缩小，因此不会重复和死循环。
//
// 6. 边界与易错点
//   不要根据 nums[middle] 与 target 的大小直接决定方向，那是 LC-704 的整体有序前提。`nums[left] <= nums[middle]` 中等号用于处理 left==middle。题目值互异非常关键；若允许重复，nums[left]==nums[mid]==nums[right] 时可能无法判断哪半有序，需要缩边界并可能退化到 O(n)。值域条件一端含等号、一端排除 middle，避免把 middle 重复保留。
//
// 7. 举一反三
//   它把二分的定义从‘数组整体有序’升级为‘每轮能证明一批候选不可能’。LC-153 会进一步去掉 target，直接把旋转断点/最小值作为边界寻找；带重复的 LC-81 则展示当判定信息不足时为什么二分会退化。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的整数。
//   输出：下标；未找到时输出 -1。
// test.in 的预期输出：4
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int search(vector<int>& nums, int target) {
        int left = 0, right = static_cast<int>(nums.size()) - 1;
        while (left <= right) {
            int middle = left + (right - left) / 2;
            if (nums[middle] == target) return middle;

            if (nums[left] <= nums[middle]) {  // 左半段有序
                if (nums[left] <= target && target < nums[middle]) right = middle - 1;
                else left = middle + 1;
            } else {                           // 右半段有序
                if (nums[middle] < target && target <= nums[right]) left = middle + 1;
                else right = middle - 1;
            }
        }
        return -1;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n, target; if (!(cin >> n >> target)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << sol.search(a, target) << "\n";
    return 0;
}
