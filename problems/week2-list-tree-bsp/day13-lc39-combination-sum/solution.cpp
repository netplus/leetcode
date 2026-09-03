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
// 约束与要求：
//   - 1 <= candidates.length <= 30
//   - 2 <= candidates[i] <= 40
//   - candidates 的所有元素互不相同
//   - 1 <= target <= 40
//
// 复杂度目标：O(2^(target/min)) 时间。
//
// ----------------------------------------------------------------------------
// 解法精讲｜组合回溯：起始下标控制去重，可重复选择
// - 核心要点：
//   1. 思路起点：候选数可无限次使用；递归参数 start 限定后续只能选择当前或更右的候选，从而消除排列顺序造成的重复。
//   2. 执行逻辑：1. 排序以便剪枝；2. 从 start 枚举，选择 candidates[i] 后仍递归 i；3. remaining 为 0 时记录，候选过大时停止。
//   3. 为什么这样做：每个组合按非降顺序生成，因此只有一条搜索路径；所有合法多重集合都可按该顺序逐项选择而被覆盖。
// - 边界与易错点：递归传 i 而非 i+1 才允许复用；官方候选值互异且为正，正数保证 remaining 剪枝成立。
// - 举一反三：若每项只能用一次改传 i+1；若候选有重复则增加同层去重，这三者构成组合回溯常用变体。
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

    void search(const vector<int>& candidates, int start, int remaining) {
        if (remaining == 0) {
            answer.push_back(path);
            return;
        }
        for (int i = start; i < static_cast<int>(candidates.size()); ++i) {
            if (candidates[i] > remaining) break;
            path.push_back(candidates[i]);
            search(candidates, i, remaining - candidates[i]);  // i 可再次使用
            path.pop_back();
        }
    }

public:
    vector<vector<int>> combinationSum(vector<int>& candidates, int target) {
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

