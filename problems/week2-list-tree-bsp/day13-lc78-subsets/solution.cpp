// ============================================================================
// LC-78：子集
// 难度：中等
// 优先级：P0（必做）
// 学习进度：第 2 周 / 第 13 天
// ----------------------------------------------------------------------------
// 题目描述：
// 给你一个整数数组 nums，数组中的元素互不相同。
// 返回该数组所有可能的子集 （幂集）。
// 解集不能包含重复的子集。
// 你可以按任意顺序返回解集。
//
// 约束与要求：
//   - 1 <= nums.length <= 10
//   - -10 <= nums[i] <= 10
//   - nums 中的所有元素互不相同
//
// 复杂度目标：O(n * 2^n)。
//
// ----------------------------------------------------------------------------
// 解法精讲｜子集回溯：每个前缀本身就是答案
// - 核心要点：
//   1. 思路起点：按下标递增选择元素；当前 path 对应一个子集，应在进入每层时记录，然后继续尝试加入后续元素。
//   2. 执行逻辑：1. 记录当前 path（包括空集）；2. 从 start 到末尾枚举下一元素；3. 选择后递归 i+1，再撤销。
//   3. 为什么这样做：每个子集按元素原下标的递增序列唯一表示，搜索树恰好枚举所有这类序列，因此共有 2^n 个且无重复。
// - 边界与易错点：不要只在叶子记录，否则会漏掉较短子集；递归传 i+1；输出规模本身就是 O(n·2^n)。
// - 举一反三：子集是组合搜索的母题；加目标和、长度、重复元素处理或可行性剪枝即可得到多种组合问题。
// ----------------------------------------------------------------------------
//
// 本地输入输出格式（用于 test.in）：
//   第 1 行：n。
//   第 2 行：n 个以空格分隔的整数。
//   输出：所有子集，每行一个且元素以空格分隔（空集输出空行）。
// test.in 的预期输出：<empty> | 1 | 2 | 1 2 | 3 | 1 3 | 2 3 | 1 2 3
// ============================================================================
#include <bits/stdc++.h>
using namespace std;


// ---------- 题解实现 ----------
class Solution {
    vector<vector<int>> answer;
    vector<int> path;

    void search(const vector<int>& nums, int start) {
        answer.push_back(path);
        for (int i = start; i < static_cast<int>(nums.size()); ++i) {
            path.push_back(nums[i]);
            search(nums, i + 1);
            path.pop_back();
        }
    }

public:
    vector<vector<int>> subsets(vector<int>& nums) {
        answer.clear();
        path.clear();
        search(nums, 0);
        return answer;
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
    auto res = sol.subsets(a);
    for (auto& s : res) {
        for (size_t i = 0; i < s.size(); ++i) cout << s[i] << " \n"[i + 1 == s.size()];
        if (s.empty()) cout << "\n";
    }
    return 0;
}

