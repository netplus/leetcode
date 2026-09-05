// ============================================================================
// LC-410：分割数组的最大值
// 难度：困难
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 23 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给定一个非负整数数组 nums 和一个整数 k，你需要将这个数组分成 k 个非空的连续子数组，使得这 k 个子数组各自和的最大值最小。
// 返回分割后最小的和的最大值。
// 子数组是数组中连续的部分。
//
// 题目示例：
// 示例 1：
//   Input: nums = [7,2,5,10,8], k = 2
//   Output: 18
//   Explanation: There are four ways to split nums into two subarrays.
//   The best way is to split it into [7,2,5] and [10,8], where the largest sum among the two subarrays is only 18.
//
// 示例 2：
//   Input: nums = [1,2,3,4,5], k = 2
//   Output: 9
//   Explanation: There are four ways to split nums into two subarrays.
//   The best way is to split it into [1,2,3] and [4,5], where the largest sum among the two subarrays is only 9.
//
// 约束与要求：
//   - 1 <= nums.length <= 1000
//   - 0 <= nums[i] <= 10^6
//   - 1 <= k <= min(50, nums.length)
//
// 复杂度目标：O(n log S) 其中 S = sum(nums)。
//
// ----------------------------------------------------------------------------
// 解法精讲｜答案二分 + 贪心判定：把最小最大段和转成最小可行上限
//
// 0. 优化是怎么来的
//   最直接的正确做法是枚举 k-1 个切分位置。数组有 n-1 个缝隙，从中选择 k-1 个后就得到一种完整分割；计算每种分割的最大段和，再取最小值。它不会漏答案，但组合数量 C(n-1,k-1) 会迅速爆炸。
//
//   与其问‘最佳切点到底在哪里’，换一个更容易回答的问题：如果我先猜答案上限 limit，能不能把数组切成 k 段，使每段和都 <= limit？因为 nums 全部非负，给定 limit 后有一个确定的最省段策略：当前元素只要还能放进本段就继续放，只有再放就超限时才切段。提前切只会让当前段更空，不可能让最终使用段数更少。
//
//   于是 check(limit) 可以 O(n) 得到最少需要多少段。limit 越大，限制越宽松，所需段数只会不增；可行 limit 再次形成 F...F | T...T。这样就从‘枚举组合切点’转成了‘二分答案 + 线性贪心判定’。优化类型：用单调决策问题替代巨大方案枚举。
//
// 1. 图像直觉
//   nums = [7,2,5,10,8], k = 2
//
//   猜 limit = 17：
//   [7,2,5] = 14
//   [10]    = 10
//   [8]     = 8
//   需要 3 段 -> 不可行
//
//   猜 limit = 18：
//   [7,2,5] = 14
//   [10,8]  = 18
//   需要 2 段 -> 可行
//
//   答案轴：
//   max(nums)=10 ........ 17 | 18 19 ... sum=32
//             false .... F | T  T  ... T
//                          ^ 第一个可行上限
//
//   它和 LC-1011 的图完全相同：
//   包裹/天数  <->  数组/分段数。
//
// 2. 一句话核心
//   不要直接搜索切点；先猜最大段和 limit，用贪心算满足该上限至少需要几段，再二分第一个 groupsNeeded(limit)<=k 的 limit。
//
// 3. 公式 / 不变量
//   搜索范围：
//   left  = max(nums)
//   right = sum(nums)
//
//   给定 limit：
//   groups = 1
//   current = 0
//   若 current + value > limit：
//       groups += 1
//       current = 0
//   随后 current += value
//
//   groupsNeeded(limit) = 在每段和 <= limit 时最少需要的段数。
//
//   limit1 <= limit2
//   => groupsNeeded(limit1) >= groupsNeeded(limit2)
//
//   feasible(limit) := groupsNeeded(limit) <= k
//   => F...F | T...T。
//
//   二分 first true。
//
// 4. 执行步骤
//   1. 用最大单元素作为答案下界、总和作为上界；任何更小上限装不下最大元素，总和则一定能作为单段上限
//   2. 给定 limit 顺序扫描 nums，当前元素能留在本段就继续累加，只有加入后会超过 limit 才切出新段
//   3. 若 groupsNeeded(middle)<=k，说明 middle 足够大，保留 middle 并继续向左寻找更小可行上限
//   4. 若需要超过 k 段，middle 以及所有更小上限都不可能，令 left=middle+1
//   5. 区间收敛后返回 left；若贪心得到少于 k 段，可继续把已有非空段拆开直到恰好 k 段，最大段和不会增加
//   记忆：不找切点，先猜上限；给定上限最晚切段，二分第一个能在 k 段内完成的值。
//
// 5. 为什么不会漏 / 不会重
//   固定 limit 时，贪心总是在‘不得不切’的最晚位置切段。任何合法方案在贪心第一段结束位置之前切，只会让第一段更短，并不能把更多元素塞进后面的固定上限，因此不可能使用更少段；对剩余后缀重复这一论证，贪心得到最少段数。由于 nums 非负，提高 limit 不会使任何原本合法的段失效，所以最少段数单调不增。若最少段数 <= k，因为 k<=n，可以继续拆分非空段而不增加任何段和，故存在恰好 k 段的合法方案。
//
// 6. 边界与易错点
//   题目要求恰好 k 个非空连续子数组，但 check 用 <=k 是正确的，原因必须建立在‘可继续拆段且 nums 非负’上。不要对数组排序，连续性和原顺序是问题结构。累加和、limit、中点都用 long long；当前实现最终结果再安全转回 int。
//
// 7. 举一反三
//   这是 LC-1011 的同构题：shipWithinDays 的 capacity 就是本题 limit，daysNeeded 就是 groupsNeeded。今后遇到‘把连续任务切成至多 k 组，最小化最大组负载’，可以先尝试直接复用这个模型；若元素允许负数，贪心最晚切段与单调性都要重新证明，不能机械套用。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n k。
//   第 2 行：n 个以空格分隔的整数。
//   输出：最小化后的最大子数组和。
// test.in 的预期输出：18
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    int splitArray(vector<int>& nums, int k) {
        // 最大段和至少要容纳最大的单个元素；把整个数组作为一段时，总和一定可行。
        long long left = *max_element(nums.begin(), nums.end());
        long long right = accumulate(nums.begin(), nums.end(), 0LL);

        auto groupsNeeded = [&](long long limit) {
            int groups = 1;
            long long current = 0;
            for (int value : nums) {
                // nums 非负。固定 limit 后，当前段能继续放就不提前切；
                // 只有加入 value 会超限时才切，得到该 limit 下所需的最少连续段数。
                if (current + value > limit) {
                    ++groups;
                    current = 0;
                }
                // 触发切分的 value 属于新段，必须在清零后继续计入。
                current += value;
            }
            return groups;
        };

        // limit 越大，最少段数只会不增，因此 groupsNeeded(limit)<=k 构成可行后缀。
        // 题目虽要求“恰好 k 段”，这里检查 <=k 仍正确：若只需少于 k 段，
        // 因为 k<=n 且 nums 非负，可以继续把非空段拆开，最大段和不会增加。
        while (left < right) {
            long long middle = left + (right - left) / 2;
            if (groupsNeeded(middle) <= k) right = middle;
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
    cout << sol.splitArray(a, k) << "\n";
    return 0;
}
