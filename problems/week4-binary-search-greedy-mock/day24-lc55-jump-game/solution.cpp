// ============================================================================
// LC-55：跳跃游戏
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 4 周 / 第 24 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个非负整数数组 nums，你最初位于数组的第一个下标。
// 数组中的每个元素代表你在该位置可以跳跃的最大长度。
// 判断你是否能够到达最后一个下标，如果可以，返回 true；否则，返回 false。
//
// 题目示例：
// 示例 1：
//   Input: nums = [2,3,1,1,4]
//   Output: true
//   Explanation: Jump 1 step from index 0 to 1, then 3 steps to the last index.
//
// 示例 2：
//   Input: nums = [3,2,1,0,4]
//   Output: false
//   Explanation: You will always arrive at index 3 no matter what. Its maximum jump length is 0, which makes it impossible to reach the last index.
//
// 约束与要求：
//   - 1 <= nums.length <= 10^4
//   - 0 <= nums[i] <= 10^5
//
// 复杂度目标：O(n) 时间，O(1) 空间（贪心）。
//
// ----------------------------------------------------------------------------
// 解法精讲｜贪心前沿：把所有可达路径压缩成最远可达位置
//
// 0. 优化是怎么来的
//   最自然的正确做法是 DFS/BFS：从每个可达位置 i 枚举 1..nums[i] 的所有下一跳，
//   直到到达末尾或搜索完所有状态。它不会漏路径，但同一个位置可能由许多跳法重复到达。
//
//   进一步观察：从某个位置继续向后的能力只取决于这个下标，而不取决于“怎么到这里”。
//   更强的是，所有已知可达位置始终形成连续前缀 [0,farthest]，所以连 visited 集合都不必保存；
//   对“最终能否到达”而言，未来只需要知道这片可达区域的最右边界。
//
//   因此把整片搜索状态压成一个整数 farthest：扫描所有 i<=farthest 的位置并持续扩张边界。
//   若 i 第一次超过 farthest，说明出现无法跨越的缺口。优化类型：重复状态消除 + 状态压缩。
//
// 1. 图像直觉
//   nums = [2,3,1,1,4]
//
//   起点 0 最多到 2：
//   [0 1 2] 3 4
//        ^ farthest=2
//
//   扫描到 i=1，它已经位于可达前缀中：1+nums[1]=4。
//   前沿因此扩成 [0 1 2 3 4]，直接覆盖终点。
//
//   反例 [3,2,1,0,4] 的前沿最终停在 3；当 i=4>farthest 时已经出现永久缺口。
//
// 2. 一句话核心
//   不枚举怎么跳，只维护所有已知可达路径共同产生的最右前沿；只有已经可达的位置才有资格继续扩张前沿。
//
// 3. 公式 / 不变量
//   初始 farthest=0。
//
//   扫描位置 i 前：
//   i <= farthest  <=>  i 已被某条路径覆盖，可以作为下一跳起点。
//
//   若 i>farthest：返回 false。
//   否则：farthest=max(farthest, i+nums[i])。
//
//   不变量：处理完 0..i 后，farthest 是只使用这些已处理起点能到达的最远位置，
//   并且 [0,farthest] 是连续可达前缀。
//
// 4. 执行步骤
//   1. farthest=0，表示起点本身已可达。
//   2. 从左到右扫描 i；若 i>farthest，立即失败。
//   3. 只有确认 i 可达后，才用 i+nums[i] 更新 farthest。
//   4. 一旦 farthest>=n-1 就立即成功。
//   记忆：路径不重要，可达前缀才重要；i 进得了前沿，才有资格把前沿推得更远。
//
// 5. 为什么不会漏 / 不会重
//   若 i<=farthest，则 i 位于已有连续可达前缀中，存在路径到达 i；从 i 新增的可达区间与旧前缀相交，
//   合并后仍是连续前缀。若 i>farthest，所有已经可达且能作为起点的位置都已扫描过，却仍到不了 i；
//   后面的下标本身尚不可达，无法凭空产生新跳跃。因此只保留 farthest 不会丢失任何可达性信息。
//
// 6. 边界与易错点
//   必须先确认 i<=farthest 再读取它的跳跃能力，否则会让实际不可达的位置错误扩张前沿。
//   长度为 1 时起点就是终点。现有实现用 long long 保存 farthest 与 i+nums[i]。
//
// 7. 举一反三
//   LC-45 在同一个前沿模型上增加“最少跳几次”：除了全局 farthest，还要保存当前跳数对应的层边界 currentEnd。
//   图论上，这相当于把 BFS 的连续 frontier 压成边界变量。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：能到达最后一个下标时输出 1，否则输出 0。
// test.in 的预期输出：1
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
public:
    bool canJump(vector<int>& nums) {
        // farthest 压缩了所有已知路径：当前已经证明连续前缀 [0,farthest] 全部可达。
        long long farthest = 0;
        for (int i = 0; i < static_cast<int>(nums.size()); ++i) {
            // 只有已经落在可达前缀中的位置才有资格贡献下一跳；
            // 第一次出现 i>farthest 时形成永久缺口，后面的未达位置不可能反过来扩张前沿。
            if (i > farthest) return false;

            // 把当前可达起点 i 的最远落点并入总前沿；中间所有更短落点都已经被连续前缀包含。
            farthest = max(farthest, static_cast<long long>(i) + nums[i]);
            if (farthest >= static_cast<int>(nums.size()) - 1) return true;
        }
        return true;
    }
};

// ---------- 本地测试适配器 ----------
int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    Solution sol;
    int n; if (!(cin >> n)) return 0;
    vector<int> a(n);
    for (int i = 0; i < n; ++i) cin >> a[i];
    cout << (sol.canJump(a) ? 1 : 0) << "\n";
    return 0;
}
