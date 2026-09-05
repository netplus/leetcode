// ============================================================================
// LC-39：组合总和
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 13 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个无重复元素的整数数组 candidates 和一个目标整数 target，找出 candidates 中可以使数字和为目标数 target 的所有不同组合，并以列表形式返回。
// 你可以按任意顺序返回这些组合。
// candidates 中的同一个数字可以无限制重复被选取。
// 如果至少一个数字的被选数量不同，则两种组合是不同的。
// 对于给定的输入，保证和为 target 的不同组合数少于 150 个。
//
// 题目示例：
// 示例 1：
//   Input: candidates = [2,3,6,7], target = 7
//   Output: [[2,2,3],[7]]
//   Explanation:
//   2 and 3 are candidates, and 2 + 2 + 3 = 7. Note that 2 can be used multiple times.
//   7 is a candidate, and 7 = 7.
//   These are the only two combinations.
//
// 示例 2：
//   Input: candidates = [2,3,5], target = 8
//   Output: [[2,2,2,2],[2,3,3],[3,5]]
//
// 示例 3：
//   Input: candidates = [2], target = 1
//   Output: []
//
// 约束与要求：
//   - 1 <= candidates.length <= 30
//   - 2 <= candidates[i] <= 40
//   - candidates 的所有元素互不相同
//   - 1 <= target <= 40
//
// 复杂度目标：O(2^(target/min)) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜组合回溯：先规定唯一构造顺序，再用正数与排序剪掉无效后缀
//
// 前置概念
//   组合（combination）与排列的关键区别是：组合不关心选择顺序。
//
//   例如 target=7 时，[2,2,3]、[2,3,2]、[3,2,2] 表示同一个组合，只能输出一次。
//
//   本题还额外允许同一个 candidate 被无限次重复使用，所以“顺序不重要”与“元素可重复使用”是两个独立规则。`start` 负责消除不同顺序的重复表示，而递归继续传 i（不是 i+1）负责允许再次选择当前值。
//
// 0. 优化是怎么来的
//   最直接的搜索可以令 remaining=target，每一层都从所有 candidates 中任选一个正数继续减；remaining==0 就得到答案。
//   它能找到答案，但同一组合会被不同顺序反复生成：2+2+3、2+3+2、3+2+2 都表示 {2,2,3}。
//
//   组合不关心顺序，所以先规定唯一规范：候选下标只能不减。选择 candidates[i] 后，下层仍从 i 开始：
//   可以再次选 i，保留“无限次使用”；不能再回头选 i 左边的候选，从根源消除排列顺序造成的重复。
//
//   再利用题目所有候选都为正数：排序后若 candidates[i] > remaining，右侧候选只会更大，整段都不可能成功，
//   可以直接 break。最后才把这种“选择 -> 递归 -> 撤销，并尽早剪掉不可能分支”的 DFS 称为回溯。
//
// 1. 图像直觉
//   candidates=[2,3,6,7], target=7：
//
//   [] rem=7 start=0
//    |
//    +-- 选2 -> [2] rem=5 start=0
//    |             +-- 选2 -> [2,2] rem=3
//    |             |             +-- 选2 -> rem=1，下一候选 2>1，停止
//    |             |             +-- 选3 -> [2,2,3] rem=0 ✓
//    |             +-- 选3 -> [2,3] rem=2，下一候选 3>2，停止
//    |
//    +-- 选3 -> [3] rem=4 start=1
//    |             后面只能继续 3/6/7，不能回头选 2
//    +-- 选7 -> [7] rem=0 ✓
//
//   选到 3 后 start 已经是 1，因此 [2,3,2] 不会产生；同一个多重集合只有非降序这一种表示。
//
// 2. 一句话核心
//   组合不关心顺序，就用 start 强制候选下标不减；当前候选可复用所以递归仍传 i，排序后候选一旦超过 remaining 就停止整段。
//
// 3. 公式 / 不变量
//   状态：search(candidates, start, remaining)
//   path 的候选下标单调不减。
//
//   remaining == 0 => 记录 path
//   本层候选 i ∈ [start,n)
//   选择 candidates[i] 后：remaining' = remaining-candidates[i]，start'=i。
//
//   candidates 已升序且全为正：
//   candidates[i] > remaining
//   => 任意 j>=i 都有 candidates[j] > remaining
//   => 当前及右侧全部不可能成功，可以 break。
//
// 4. 执行步骤
//   1. 先排序 candidates；它既建立组合的统一非降序表示，也让“过大后 break”成为合法剪枝
//   2. 从 start 枚举候选 i；若 candidates[i]>remaining，利用排序证明后续更大候选也都失败，停止本层
//   3. push candidates[i]，递归 search(candidates,i,remaining-candidates[i])；仍传 i 表示当前值可重复使用
//   4. remaining==0 时复制 path；递归回来后 pop，恢复父状态再试下一个候选
//   记忆：组合先规定唯一顺序；可复用传 i，只用一次传 i+1；排序后大于 remaining 就整段停。
//
// 5. 为什么不会漏 / 不会重
//   任意合法组合都可按排序后的候选下标非降序排列；搜索允许从 start 选当前或更右候选，并允许再次选择 i，
//   所以这条规范路径一定存在。反过来，每条路径下标都不减，同一个多重集合只有这一种非降序排列，所以不会重。
//   剪枝也安全：当前候选已大于 remaining 时，右侧更大的正数更不可能让 remaining 恰好归零。
//
// 6. 边界与易错点
//   允许重复使用当前候选，所以递归必须传 i；误传 i+1 就变成“每个元素最多一次”。`break` 依赖排序；不排序时
//   不能据一个过大值证明后面的值也过大。当前实现会原地 sort(candidates)，这是可观察副作用。题目保证候选互异且为正；
//   若有重复值需要额外同层去重，若允许非正数则 remaining 严格下降与当前剪枝都不再成立。
//
// 7. 举一反三
//   直接从 LC-78 迁移：两题都用 start 规定组合的唯一顺序。LC-78 选过 i 后传 i+1，因为元素只能用一次；
//   LC-39 传 i，因为当前值可以无限复用。组合题先问：顺序算不算不同答案？候选能用几次？输入是否有重复值？
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n target。
//   第 2 行：n 个以空格分隔的候选数。
//   输出：所有组合，每行一个且元素以空格分隔。
// test.in 的预期输出：2 2 3 | 7
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;

    // start 让 path 中使用的候选下标保持不减，给“组合”规定唯一的非降序构造顺序；
    // remaining 表示当前路径距离 target 还差多少。
    void search(const vector<int>& candidates, int start, int remaining) {
        // remaining 恰好归零时，当前 path 才是一组完整答案；所有候选为正数，所以之后无需继续扩展。
        if (remaining == 0) {
            answer.push_back(path);
            return;
        }

        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            // candidates 已升序；若当前值都超过 remaining，右侧候选只会更大，
            // 且所有数为正，不可能靠继续选择把 remaining 拉回 0，因此可直接停止本层。
            if (candidates[i] > remaining) break;

            path.push_back(candidates[i]);

            // 与 LC-78 的 i+1 不同：本题允许同一候选无限复用，所以递归仍传 i；
            // 但不会传更小下标，从而不会生成 [2,3,2] 这类同一组合的另一排列。
            search(candidates, i, remaining - candidates[i]);

            // 当前候选对应的整棵子树探索完后撤销，恢复父状态再尝试下一个候选。
            path.pop_back();
        }
    }

public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
        // 排序有两个作用：建立组合的统一非降序表示，并使“过大就 break”的剪枝有单调性依据。
        // 这会原地改变调用方传入的 candidates 顺序，是当前实现可观察的副作用。
        sort(candidates.begin(), candidates.end());

        answer.clear();
        path.clear();
        search(candidates, 0, target);
        return answer;
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
    auto res = sol.combinationSum(a, target);
    for (auto& c : res) {
        for (size_t i = 0; i < c.size(); ++i) cout << c[i] << " \n"[i + 1 == c.size()];
    }
    return 0;
}
